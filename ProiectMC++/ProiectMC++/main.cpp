#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <iostream>
#include "Map.h"
#include "Bullet.h"
#include "Direction.h"
#include "Player.h"
#include <crow.h>
#include <Windows.h>
#include <sqlite_orm/sqlite_orm.h>
#include "GameDatabase.h"

using namespace sqlite_orm;

char getPressedKey() {
	if (GetAsyncKeyState('W') & 0x8000) return 'W';
	if (GetAsyncKeyState('S') & 0x8000) return 'S';
	if (GetAsyncKeyState('A') & 0x8000) return 'A';
	if (GetAsyncKeyState('D') & 0x8000) return 'D';
	return 0; 
}

int main()
{
	Map gameMap;
	gameMap.GenerateMap();
	std::cout << "Harta generata:\n";
	//gameMap.Draw();
	std::pair<size_t, size_t> bombPosition{ 3, 3 };
	gameMap.SetTile(bombPosition, TileType::DestrucitbleWallWithBomb);
	std::cout << "Harta generata cu bomba :\n";
	//gameMap.Draw();
	gameMap.DestroyTile(bombPosition);
	std::cout << "Harta generata cu bomba dupa explozie:\n";
	//gameMap.Draw();

	std::pair<size_t, size_t> initialPosition = { 5, 5 };
	Direction bulletDirection = Direction::Left;

	Bullet bullet(initialPosition, bulletDirection);

	auto pos = bullet.getPosition();
	std::cout << "Pozitia initiala a glontului: (" << pos.first << ", " << pos.second << ")\n";
	bullet.MoveBullet();

	pos = bullet.getPosition();
	std::cout << "Pozitia glontului dupa miscare: (" << pos.first << ", " << pos.second << ")\n";

	Player player;
	player.setPosition(gameMap.getStartPosition(0));
	gameMap.SetPlayerPosition(0,player.getPosition());
	gameMap.SetTile(player.getPosition(), TileType::Player);
	while(true)
	{
		gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::EmptySpace);
		switch (getPressedKey()) {
		case 'W':
			if (player.getPosition().first > 0 &&
				gameMap.GetTile({player.getPosition().first-1,player.getPosition().second}) == TileType::EmptySpace)
					player.move('W');
			break;
		case 'S':
			if (player.getPosition().first < gameMap.getHeight()-1 &&
				gameMap.GetTile({ player.getPosition().first + 1,player.getPosition().second }) == TileType::EmptySpace)
					player.move('S');
			break;
		case 'A':
			if (player.getPosition().second > 0 &&
				gameMap.GetTile({ player.getPosition().first,player.getPosition().second-1 }) == TileType::EmptySpace)
					player.move('A');
			break;
		case 'D':
			if (player.getPosition().second < gameMap.getWidth()-1 &&
				gameMap.GetTile({ player.getPosition().first ,player.getPosition().second+1 }) == TileType::EmptySpace)
					player.move('D');
			break;
		default:
			break;
		}
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
