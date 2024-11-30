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

void UpdateBullets(std::vector<Bullet>& bullets, std::vector<Wall>& walls, Map& gameMap) {
	for (auto& bullet : bullets) {
		if (bullet.IsActive()) {
			bullet.MoveBullet();
			bullet.CheckBulletWallCollisions(walls, gameMap);
			if (bullet.IsActive()) {
				gameMap.SetTile(bullet.getPosition(), TileType::Bullet);
			}
		}
	}
}


void handleInput(const char& key, Player& player, Map& gameMap, std::vector<Bullet>& bullets)
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
			try {
				bullets.push_back(player.shoot());
			}
			catch (const std::runtime_error& e) {
				std::cerr << "Cooldown active: " << e.what() << std::endl;
			}
			return;
		default:
			return;
		}

		if ((newPosition.first >= 0 && newPosition.first <= gameMap.getHeight() - 1)
			&& (newPosition.second >= 0 && newPosition.second <= gameMap.getWidth() - 1) && gameMap.GetTile(newPosition) == TileType::EmptySpace) {
			player.move(key);
		}
	}
}
			

int main()
{
	Map gameMap;
	gameMap.GenerateMap();

	std::vector<Bullet> bullets;
	Player player;
	player.setPosition(gameMap.getStartPosition(0));
	gameMap.SetPlayerPosition(0,player.getPosition());
	gameMap.SetTile(player.getPosition(), TileType::Player);
	gameMap.Draw();
	while(true)
	{
		for (const auto& bullet : bullets) {
			if (bullet.IsActive()) {
				gameMap.SetTile(bullet.getPosition(), TileType::EmptySpace);
			}
		}
		gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::EmptySpace);
		handleInput('A', player, gameMap, bullets);
		handleInput('D', player, gameMap, bullets);
		handleInput('W', player, gameMap, bullets);
		handleInput('S', player, gameMap, bullets);
		handleInput(VK_SPACE, player, gameMap, bullets);
		gameMap.SetPlayerPosition(0, player.getPosition());
		gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::Player);
		std::vector<Wall> walls = gameMap.GetWalls();
		UpdateBullets(bullets, walls, gameMap);
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
