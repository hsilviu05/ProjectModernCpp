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

using namespace sqlite_orm;



			

int main()
{

    //Game game;
    //game.start();
    //Player player;
    //std::cout <<player.shoot()<< player.CanShoot();
    //Map gameMap;
    //gameMap.GenerateMap();

    //Player player;
    //BulletManager bulletManager;
    //player.setPosition(gameMap.getStartPosition(0));
    //player.SetPlayerID(0);
    //gameMap.SetPlayerPosition(0,player.getPosition());
    //gameMap.SetTile(player.getPosition(), TileType::Player);
    //while(true)
    //{
    //	gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::EmptySpace);
    //	//handleInput('A', player, gameMap,bulletManager);
    //	//handleInput('D', player, gameMap,bulletManager);
    //	//handleInput('W', player, gameMap,bulletManager);
    //	//handleInput('S', player, gameMap,bulletManager);
    //	//handleInput(VK_SPACE, player, gameMap,bulletManager);
    //	bulletManager.UpdateBullets(gameMap);
    //	gameMap.SetPlayerPosition(0, player.getPosition());
    //	gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::Player);
    //	gameMap.Draw();
    //	Sleep(200);

    /*
    crow::SimpleApp app;

    Map map;
    map.GenerateMap();

    CROW_ROUTE(app, "/map")
        .methods("GET"_method)
        ([&map]() {
        crow::json::wvalue result;
        result["height"] = map.getHeight();
        result["width"] = map.getWidth();

        crow::json::wvalue::list mapArray;
        for (size_t i = 0; i < map.getHeight(); ++i) {
            crow::json::wvalue::list rowArray;
            for (size_t j = 0; j < map.getWidth(); ++j) {
                rowArray.push_back(static_cast<int>(map.GetTile({ i, j })));
            }
            mapArray.push_back(std::move(rowArray));
        }
        result["map"] = std::move(mapArray);

        crow::json::wvalue::list wallsArray;
        auto walls = map.GetWalls();
        for (const auto& wall : walls) {
            crow::json::wvalue wallJson;
            wallJson["x"] = wall.getPosition().first;
            wallJson["y"] = wall.getPosition().second;
            wallJson["type"] = static_cast<int>(wall.getWallType());
            wallsArray.emplace_back(std::move(wallJson));
        }
        result["walls"] = std::move(wallsArray);

        return crow::response(result);
            });

    app.port(18080).multithreaded().run();
    */
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

        app.port(18080).multithreaded().run();
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
