import Direction;
#include <iostream>
#include "Map.h"
#include "Bullet.h"
#include "Player.h"
#include <crow.h>
#include <Windows.h>
#include <sqlite_orm/sqlite_orm.h>
#include "Game.h"
#include"TileType.h"

#include "BulletManager.h"


#include "..\LoggingDLL\AccountManager.h"

using namespace sqlite_orm;

std::vector<Player> lobby;
std::mutex lobbyMutex;
std::condition_variable lobbyCondition;

/*
void startGame() {
    Game game;
	Map map;

    {
        std::unique_lock<std::mutex> lock(lobbyMutex);
        // Initialize players in the game from the lobby
        for (size_t i = 0; i < lobby.size(); ++i) {
            lobby[i].SetPlayerID(i);
            //auto startPosition = map.getStartPosition(lobby[i].GetPlayerID());
            std::pair<size_t, size_t> startPosition = map.getStartPosition(lobby[i].GetPlayerID());
            lobby[i].SetPosition(startPosition);
            map.SetPlayerPosition(lobby[i].GetPlayerID(), startPosition);
            map.SetTile(startPosition, TileType::Player);
        }
        lobby.clear(); // Clear the lobby
    }

    game.start();
}
*/


int main()
{
    AccountManager account;
    const std::string dbFile = "account_data.db";

    Map map;  // Assuming the Map object is instantiated
    map.GenerateMap();  // Generate the map

    //Game game;
    //game.start();
    //return 0;


    
    std::mutex mapMutex;  // Mutex for thread-safety

    crow::SimpleApp app;
    AccountManager accountManager;


    // Route pentru înregistrare
    CROW_ROUTE(app, "/signup").methods(crow::HTTPMethod::POST)([&accountManager](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);

        std::string username = x["username"].s();
        std::string password = x["password"].s();
        std::string dbFile = "account_data.db";

        try {
            accountManager.SignUp(dbFile, username, password);
            return crow::response(200, "Account created successfully!");
        }
        catch (const std::exception& e) {
            return crow::response(403, e.what());
        }
        });

    

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([&accountManager, &map](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);

        std::string username = x["username"].s();
        std::string password = x["password"].s();
        std::string dbFile = "account_data.db";

        try {
            accountManager.LoadDataFromDatabase(dbFile, username);

            if (accountManager.Authenticate(username, password)) {
                uint16_t points = accountManager.GetPoints();

                std::chrono::milliseconds fireRate(accountManager.GetFireRate());
                uint8_t fireRateUpgrades = GameSettings::MAX_FIRE_RATE_UPGRADES;
                double bulletSpeed = GameSettings::DEFAULT_BULLET_SPEED;
                bool bulletSpeedUpgraded = accountManager.GetSpeedBoost();

                std::unique_lock<std::mutex> lock(lobbyMutex);
                auto it = std::find_if(lobby.begin(), lobby.end(), [&username](const Player& player) {
                    return player.GetUsername() == username;
                    });

                if (it == lobby.end()) {
                    Player player(username, fireRate, fireRateUpgrades, bulletSpeed, bulletSpeedUpgraded); // Create a Player instance
                    player.SetPlayerID(lobby.size());
                    auto startPosition = map.getStartPosition(player.GetPlayerID());
                    player.SetPosition(startPosition);
                    map.SetPlayerPosition(player.GetPlayerID(), startPosition);
                    map.SetTile(startPosition, TileType::Player);
                    player.Respawn(startPosition);
                    lobby.push_back(player);
                }

                if (lobby.size() == 4) {
                    lobbyCondition.notify_one(); // Notify the game-start thread
                }
                return crow::response(200, "Login successful!");
            }
            else {
                return crow::response(403, "Invalid credentials.");
            }
        }
        catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
        });


    std::thread gameThread([]() {
        while (true) {
            std::unique_lock<std::mutex> lock(lobbyMutex);
            lobbyCondition.wait(lock, []() { return lobby.size() == 4; });
            //startGame();
        }
        });


    // Route for joining a game
    CROW_ROUTE(app, "/join_game").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);


        std::string username = x["username"].s();
        std::cout << "Received join_game request for username: " << username << std::endl;

        if (username.empty()) {
            std::cout << "Username is empty" << std::endl;
            return crow::response(400, "Missing username in request body");
        }

        std::unique_lock<std::mutex> lock(lobbyMutex);

        // Find player in lobby or create new player
        auto it = std::find_if(lobby.begin(), lobby.end(), [&username](const Player& player) {
            return player.GetUsername() == username;
            });
        /*
        if (it == lobby.end()) {
            std::chrono::milliseconds fireRate(300);
            uint8_t fireRateUpgrades = 3;
            double bulletSpeed = 1.0;
            bool bulletSpeedUpgraded = false;
            Player newPlayer(username, fireRate, fireRateUpgrades, bulletSpeed, bulletSpeedUpgraded);
            lobby.push_back(newPlayer);
        }
        */
        if (lobby.size() >= 4) {
            lobbyCondition.notify_one(); // Notify the game-start thread
        }

        return crow::response(200);
        });

    // Route for checking match status
    CROW_ROUTE(app, "/check_match").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        std::unique_lock<std::mutex> lock(lobbyMutex);
        auto username = req.url_params.get("username");

        auto it = std::find_if(lobby.begin(), lobby.end(), [&username](const Player& player) {
            return player.GetUsername() == username;
            });

        if (it != lobby.end()) {
            crow::json::wvalue result;
            result["match_found"] = true;
            result["player_id"] = it->GetPlayerID();
            // Fill in player position or other initial game state
            result["x"] = it->getPosition().first;; // Example values
            result["y"] = it->getPosition().second; // Example values
            return crow::response(result);
        }
        else {
            return crow::response(200, R"({"match_found": false})");
        }
        });

    CROW_ROUTE(app, "/map")
        .methods("GET"_method)
        ([&map, &mapMutex]() {
        std::lock_guard<std::mutex> lock(mapMutex); // Lock the map during the entire operation

        crow::json::wvalue result;
        result["height"] = map.GetHeight();
        result["width"] = map.GetWidth();

        // Serialize the map tiles into JSON
        crow::json::wvalue::list mapArray;
        crow::json::wvalue::list wallsArray;

        for (size_t i = 0; i < map.GetHeight(); ++i) {
            crow::json::wvalue::list rowArray;
            for (size_t j = 0; j < map.GetWidth(); ++j) {
                TileType tile = map.GetTile({ i, j });
                rowArray.push_back(static_cast<int>(tile));

                // Check for wall types and add to wallsArray
                if (tile == TileType::DestrucitbleWall ||
                    tile == TileType::IndestrucitbleWall ||
                    tile == TileType::DestrucitbleWallWithBomb) {
                    crow::json::wvalue wallJson;
                    wallJson["x"] = i;
                    wallJson["y"] = j;
                    wallJson["type"] = static_cast<int>(tile);
                    wallsArray.emplace_back(std::move(wallJson));
                }
            }
            mapArray.push_back(std::move(rowArray));
        }

        result["map"] = std::move(mapArray);
        result["walls"] = std::move(wallsArray);

        return crow::response(result);
            });

    CROW_ROUTE(app, "/move").methods(crow::HTTPMethod::POST)([&map, &mapMutex](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);

        size_t newX = x["x"].i();
        size_t newY = x["y"].i();
        size_t playerID = x["playerID"].i();
        if (playerID >= 4) return crow::response(403, "Invalid playerID.");

        {
            std::lock_guard<std::mutex> lock(mapMutex);

            // Check if the new position is valid (within bounds and not blocked)
            std::pair<size_t, size_t> newPosition(newX, newY);
            if (!map.InBounds(newPosition) || map.GetTile(newPosition) != TileType::EmptySpace) {
                return crow::response(403, "Invalid move.");
            }


            auto oldPosition = map.GetPlayerPosition(playerID);
            map.SetTile(oldPosition, TileType::EmptySpace);
            // Update player's position
            map.SetPlayerPosition(playerID, newPosition);
            map.SetTile(newPosition, TileType::Player);  // Mark the new position as player position

            // Optional: If previous position was set to a player tile, mark it as empty
            /*auto oldPosition = map.GetPlayerPosition(playerID);
            map.SetTile(oldPosition, TileType::EmptySpace); */// Clear the previous position
        }

        // Notify the client that the move was successful and return the updated map
        crow::json::wvalue result;
        result["status"] = "Move successful";

        // Now return the updated map after the move
        crow::json::wvalue::list mapArray;
        for (size_t i = 0; i < map.GetHeight(); ++i) {
            crow::json::wvalue::list rowArray;
            for (size_t j = 0; j < map.GetWidth(); ++j) {
                TileType tile = map.GetTile({ i, j });
                rowArray.push_back(static_cast<int>(tile));
            }
            mapArray.push_back(std::move(rowArray));
        }

        result["map"] = std::move(mapArray);

        crow::json::wvalue::list playersArray;
        for (size_t i = 0; i < 4; ++i) { // presupunem 4 jucători
            auto position = map.GetPlayerPosition(i);
            crow::json::wvalue player;
            player["id"] = i;
            player["x"] = position.first;
            player["y"] = position.second;
            playersArray.push_back(std::move(player));
        }
        result["players"] = std::move(playersArray);

        return crow::response(result);
        });



     

}


//CROW_ROUTE(app, "/move")
//    .methods("POST"_method)
//    ([&map](const crow::request& req) {
//    auto jsonReq = crow::json::load(req.body);
//    if (!jsonReq) {
//        return crow::response(400, "Invalid JSON");
//    }

//    int x = jsonReq["x"].i();
//    int y = jsonReq["y"].i();
//    int playerID = jsonReq["playerID"].i();

//    // Validarea noii poziții
//    std::pair<size_t, size_t> newPosition = { static_cast<size_t>(x), static_cast<size_t>(y) };
//    if (map.InBounds(newPosition) && map.GetTile(newPosition) == TileType::EmptySpace) {
//        // Update the player's position
//        map.SetTile(map.GetPlayerPosition(playerID), TileType::EmptySpace);
//        map.SetPlayerPosition(playerID, newPosition);
//        map.SetTile(newPosition, TileType::Player);
//        playerPosition = newPosition;
//    }

//    crow::json::wvalue result;
//    result["x"] = playerPosition.first;
//    result["y"] = playerPosition.second;
//    return crow::response(result);
//        });

//Player player(1, 1);  // poziția de start (1, 1)
//map.SetTile({ 1, 1 }, TileType::Player);

//CROW_ROUTE(app, "/move")
//    .methods("POST"_method)
//    ([&map, &player, &mapMutex](const crow::request& req) {
//    std::lock_guard<std::mutex> lock(mapMutex);

//    auto body = crow::json::load(req.body);
//    if (!body) {
//        return crow::response(400, "Invalid JSON");
//    }

//    // Verificăm dacă "direction" este un string și dacă are cel puțin un caracter
//    if (body["direction"].t() != crow::json::type::String || body["direction"].s().length() == 0) {
//        return crow::response(400, "Invalid direction");
//    }
//    char direction = body["direction"].s()[0];
//    player.move(direction);

//    // Actualizăm harta
//    auto playerPos = player.getPosition();
//    map.SetTile(playerPos, TileType::Player);

//    // Construim obiectul JSON pentru "new_position"
//    crow::json::wvalue new_position;
//    new_position["x"] = playerPos.first;
//    new_position["y"] = playerPos.second;

//    crow::json::wvalue result;
//    result["new_position"] = new_position;

//    return crow::response(result);
//        });




    //app.port(18080).multithreaded().run();

