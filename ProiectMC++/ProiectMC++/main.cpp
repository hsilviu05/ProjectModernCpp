#include <iostream>
#include "Map.h"
#include "Bullet.h"
#include "Direction.h"
#include "Player.h"
#include <crow.h>
#include <Windows.h>
#include <sqlite_orm/sqlite_orm.h>
#include "Game.h"

#include "BulletManager.h"
#include "GameDatabase.h"

using namespace sqlite_orm;



			

int main()
{
	Map gameMap;
	gameMap.GenerateMap();

	Player player;
	BulletManager bulletManager;
	player.setPosition(gameMap.getStartPosition(0));
	player.SetPlayerID(0);
	gameMap.SetPlayerPosition(0,player.getPosition());
	gameMap.SetTile(player.getPosition(), TileType::Player);
	while(true)
	{
		gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::EmptySpace);
		//handleInput('A', player, gameMap,bulletManager);
		//handleInput('D', player, gameMap,bulletManager);
		//handleInput('W', player, gameMap,bulletManager);
		//handleInput('S', player, gameMap,bulletManager);
		//handleInput(VK_SPACE, player, gameMap,bulletManager);
		bulletManager.UpdateBullets(gameMap);
		gameMap.SetPlayerPosition(0, player.getPosition());
		gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::Player);
		gameMap.Draw();
		Sleep(200);

	}

	crow::SimpleApp app;
	Storage storage = createStorage("product.sqlite");
	


	
	CROW_ROUTE(app, "/register")
		.methods("POST"_method)([&storage](const crow::request& req) {
		auto json = crow::json::load(req.body);
		if (!json) return crow::response(400, "Invalid JSON");

		std::string username = json["username"].s();
		std::string name = json["name"].s();

		// Verificăm dacă username-ul este deja în uz
		auto existingPlayer = storage.get_all<PlayerDB>(where(c(&PlayerDB::username) == username));
		if (!existingPlayer.empty()) {
			return crow::response(409, "Username already exists");
		}

		// Creăm un nou utilizator
		PlayerDB player{ -1, username, name, 0, 0 };
		storage.insert(player);

		return crow::response(200, "User registered successfully");
			});


	CROW_ROUTE(app, "/login")
		.methods("POST"_method)([&storage](const crow::request& req) {
		auto json = crow::json::load(req.body);
		if (!json) return crow::response(400, "Invalid JSON");

		std::string username = json["username"].s();

		// Căutăm utilizatorul în baza de date
		auto existingPlayer = storage.get_all<PlayerDB>(where(c(&PlayerDB::username) == username));
		if (existingPlayer.empty()) {
			return crow::response(404, "User not found");
		}

		return crow::response(200, "Login successful");
			});
				
	return 0;
}
