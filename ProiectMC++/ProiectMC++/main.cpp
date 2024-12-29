#include <iostream>
#include "Map.h"
#include "Bullet.h"
#include "Direction.h"
#include "Player.h"
#include <crow.h>
#include <Windows.h>
#include <sqlite_orm/sqlite_orm.h>
#include "Game.h"
#include"TileType.h"

#include "BulletManager.h"
#include "GameDatabase.h"

#include "AccountManager.h"

using namespace sqlite_orm;



			
std::pair<int, int> playerPosition = { 0, 0 };

int main()
{

    AccountManager account("username", "password");

    // Authenticate the user
    if (account.authenticate("username", "password")) {
        std::cout << "Authentication successful!" << std::endl;
    }
    else {
        std::cout << "Authentication failed!" << std::endl;
    }

    // Save data to file
    account.saveDataToFile("account_data.txt");

    // Load data from file
    AccountManager loadedAccount;
    loadedAccount.loadDataFromFile("account_data.txt");

    std::cout << "Loaded Account: " << loadedAccount.getUsername() << std::endl;

    /*Game game;
    game.start();*/
    return 0;
    /*

    std::mutex mapMutex;  // Mutex for thread-safety

    crow::SimpleApp app;

    Map map;  // Assuming the Map object is instantiated
    map.GenerateMap();  // Generate the map

    CROW_ROUTE(app, "/map")
        .methods("GET"_method)
        ([&map, &mapMutex]() {
        std::lock_guard<std::mutex> lock(mapMutex); // Lock the map during the entire operation

        crow::json::wvalue result;
        result["height"] = map.getHeight();
        result["width"] = map.getWidth();

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

    CROW_ROUTE(app, "/move")
        .methods("POST"_method)
        ([&map](const crow::request& req) {
        auto jsonReq = crow::json::load(req.body);
        if (!jsonReq) {
            return crow::response(400, "Invalid JSON");
        }

        int x = jsonReq["x"].i();
        int y = jsonReq["y"].i();
        int playerID = jsonReq["playerID"].i();

        // Validarea noii poziții
        std::pair<size_t, size_t> newPosition = { static_cast<size_t>(x), static_cast<size_t>(y) };
        if (map.inBounds(newPosition) && map.GetTile(newPosition) == TileType::EmptySpace) {
            // Update the player's position
            map.SetTile(map.GetPlayerPosition(playerID), TileType::EmptySpace);
            map.SetPlayerPosition(playerID, newPosition);
            map.SetTile(newPosition, TileType::Player);
            playerPosition = newPosition;
        }

        crow::json::wvalue result;
        result["x"] = playerPosition.first;
        result["y"] = playerPosition.second;
        return crow::response(result);
            });



        app.port(18080).multithreaded().run();
    */
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
