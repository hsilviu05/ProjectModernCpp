﻿#include <iostream>
#include "Map.h"
#include "Bullet.h"
#include "Direction.h"
#include "Player.h"
#include <crow.h>
#include <Windows.h>
#include <sqlite_orm/sqlite_orm.h>
#include "Game.h"
#include"TileType.h"
#include <thread>
#include <mutex>

#include "BulletManager.h"
#include "GameDatabase.h"

#include "..\LoggingDLL\AccountManager.h"

using namespace sqlite_orm;

std::mutex db_mutex;

static void handle_signup(const std::string& dbFile, const std::string& username, const std::string& password, crow::response& res) {
    std::lock_guard<std::mutex> lock(db_mutex);
    AccountManager accountManager;

    try {
        accountManager.SignUp(dbFile, username, password);
        accountManager.LoadDataFromDatabase(dbFile, username);

        auto accountJson = accountManager.To_json();
        res.code = 200;
        res.write(accountJson.dump()); // Trimite informațiile contului în format JSON
    }
    catch (const std::exception& e) {
        res.code = 500; // Eroare internă de server
        res.write("Error during signup: " + std::string(e.what()));
    }
    res.end();
}

static void handle_login(const std::string& dbFile, const std::string& username, const std::string& password, crow::response& res) {
    std::lock_guard<std::mutex> lock(db_mutex);
    AccountManager accountManager;

    try {
        accountManager.LoadDataFromDatabase(dbFile, username);

        if (accountManager.Authenticate(username, password)) {
            auto accountJson = accountManager.To_json();
            res.code = 200;
            res.write(accountJson.dump()); // Trimite informațiile contului în format JSON
        }
        else {
            res.code = 401; // Neautorizat
            res.write("Invalid credentials. Please try again.");
        }
    }
    catch (const std::exception& e) {
        res.code = 500; // Eroare internă de server
        res.write("Error during login: " + std::string(e.what()));
    }
    res.end();
}

int main()
{
    const std::string dbFile = "..\..\ProiectMC++\ProiectMC++\_account_data.db";
    
    Game game;
    game.start();
    return 0;

    /**/
    /*

    std::mutex mapMutex;  // Mutex for thread-safety

    crow::SimpleApp app;     

    CROW_ROUTE(app, "/signup").methods("GET"_method)([&dbFile](const crow::request& req, crow::response& res) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password") || body["username"].s().size()== 0 || body["password"].s().size() == 0) {
            res.code = 400;
            res.write("Invalid input. Username and password are required.");
            res.end();
            return;
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        handle_signup(dbFile, username, password, res);
        });

    CROW_ROUTE(app, "/login").methods("GET"_method)([&dbFile](const crow::request& req, crow::response& res) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password") || body["username"].s().size() == 0 || body["password"].s().size() == 0) {
            res.code = 400;
            res.write("Invalid input. Username and password are required.");
            res.end();
            return;
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        handle_login(dbFile, username, password, res);
        });


    Map map;
    map.GenerateMap();

        

    CROW_ROUTE(app, "/map")
        .methods("GET"_method)
        ([&map, &mapMutex]() {
        std::lock_guard<std::mutex> lock(mapMutex); // Lock the map during the entire operation

        crow::json::wvalue result;
        result["height"] = map.getHeight();  // Map height
        result["width"] = map.getWidth();    // Map width

        // Serialize the map tiles into JSON
        crow::json::wvalue::list mapArray;
        crow::json::wvalue::list wallsArray;

        for (size_t i = 0; i < map.getHeight(); ++i) {
            crow::json::wvalue::list rowArray;
            for (size_t j = 0; j < map.getWidth(); ++j) {
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


	*/


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
    
}


//	crow::SimpleApp app;
//	Storage storage = createStorage("product.sqlite");
//	
//
//
//	
//	CROW_ROUTE(app, "/register")
//		.methods("POST"_method)([&storage](const crow::request& req) {
//		auto json = crow::json::load(req.body);
//		if (!json) return crow::response(400, "Invalid JSON");
//
//		std::string username = json["username"].s();
//		std::string name = json["name"].s();
//
//		// Verificăm dacă username-ul este deja în uz
//		auto existingPlayer = storage.get_all<PlayerDB>(where(c(&PlayerDB::username) == username));
//		if (!existingPlayer.empty()) {
//			return crow::response(409, "Username already exists");
//		}
//
//		// Creăm un nou utilizator
//		PlayerDB player{ -1, username, name, 0, 0 };
//		storage.insert(player);
//
//		return crow::response(200, "User registered successfully");
//			});
//
//
//	CROW_ROUTE(app, "/login")
//		.methods("POST"_method)([&storage](const crow::request& req) {
//		auto json = crow::json::load(req.body);
//		if (!json) return crow::response(400, "Invalid JSON");
//
//		std::string username = json["username"].s();
//
//		// Căutăm utilizatorul în baza de date
//		auto existingPlayer = storage.get_all<PlayerDB>(where(c(&PlayerDB::username) == username));
//		if (existingPlayer.empty()) {
//			return crow::response(404, "User not found");
//		}
//
//		return crow::response(200, "Login successful");
//			});
//				
//	return 0;
//}
