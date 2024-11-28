#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <iostream>
#include "Map.h"
#include "Bullet.h"
#include "Direction.h"
#include "Player.h"
#include <crow.h>
#include <Windows.h>
#include <sqlite_orm/sqlite_orm.h>

#include "BulletManager.h"
#include "GameDatabase.h"

using namespace sqlite_orm;


void handleInput(const char& key, Player& player, Map& gameMap,BulletManager& bulletManager)
{
	if (GetAsyncKeyState(key) & 0x8000) {
		std::pair<size_t, size_t> newPosition = player.getPosition();
		switch (key) {
		case 'A':
			newPosition.second -= 1;
			player.setDirection(Direction::Left);
			break;
		case 'D':
			newPosition.second += 1;
			player.setDirection(Direction::Right);
			break;
		case 'W':
			newPosition.first -= 1;
			player.setDirection(Direction::Up);
			break;
		case 'S':
			newPosition.first += 1;
			player.setDirection(Direction::Down);
			break;
		case VK_SPACE:
			bulletManager.AddBullet(player.shoot());

			break;
		default:
			return;
		}

		if (gameMap.inBounds(newPosition)==true && gameMap.GetTile(newPosition) == TileType::EmptySpace) {
			player.move(key);
		}
	}
}
			

int main()
{
	Map gameMap;
	gameMap.GenerateMap();
	std::cout << "Harta generata:\n";
	std::pair<size_t, size_t> bombPosition{ 3, 3 };
	gameMap.SetTile(bombPosition, TileType::DestrucitbleWallWithBomb);
	//std::cout << "Harta generata cu bomba :\n";
	//gameMap.Draw();
	gameMap.DestroyTile(bombPosition);
	//std::cout << "Harta generata cu bomba dupa explozie:\n";
	//gameMap.Draw();

	std::pair<size_t, size_t> initialPosition = { 5, 5 };
	Direction bulletDirection = Direction::Left;

	//Bullet bullet(initialPosition, bulletDirection);

	//auto pos = bullet.getPosition();
	//std::cout << "Pozitia initiala a glontului: (" << pos.first << ", " << pos.second << ")\n";
	//bullet.MoveBullet();

	//pos = bullet.getPosition();
	//std::cout << "Pozitia glontului dupa miscare: (" << pos.first << ", " << pos.second << ")\n";


	Player player;
	BulletManager bulletManager;
	player.setPosition(gameMap.getStartPosition(0));
	player.SetPlayerID(0);
	gameMap.SetPlayerPosition(0,player.getPosition());
	gameMap.SetTile(player.getPosition(), TileType::Player);
	gameMap.Draw();
	while(true)
	{
		gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::EmptySpace);
		handleInput('A', player, gameMap,bulletManager);
		handleInput('D', player, gameMap,bulletManager);
		handleInput('W', player, gameMap,bulletManager);
		handleInput('S', player, gameMap,bulletManager);
		handleInput(VK_SPACE, player, gameMap,bulletManager);
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
