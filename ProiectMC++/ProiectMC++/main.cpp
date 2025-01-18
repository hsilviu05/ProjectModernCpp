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
std::vector<Bullet> activeBullets;

#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this, i] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        cv.wait(lock, [this] { return !tasks.empty() || stop; });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();  // Execute the task
                }
                });
        }
    }

    // Add tasks to the pool
    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::move(task));
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        cv.notify_all();
        for (auto& worker : workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool stop = false;
};

int main()
{
    AccountManager account;
    const std::string dbFile = "account_data.db";

    Map map;  // Assuming the Map object is instantiated
    map.GenerateMap();  // Generate the map

    std::mutex mapMutex;  // Mutex for thread-safety
    std::mutex bulletsMutex;

    crow::SimpleApp app;
    AccountManager accountManager;

    std::atomic<bool> isShootActive{ false };

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
                uint16_t fireRateUpgrades = accountManager.GetFireRateUpgrades();
                bool isbulletSpeedUpgraded = accountManager.GetIsSpeedUpgrade();
                double bulletSpeed;
                if (!isbulletSpeedUpgraded) {
                    bulletSpeed = GameSettings::DEFAULT_BULLET_SPEED;
                }
                else {
                    bulletSpeed = GameSettings::BULLET_SPEED_UPGRADED;
                }

                std::unique_lock<std::mutex> lock(lobbyMutex);
                auto it = std::find_if(lobby.begin(), lobby.end(), [&username](const Player& player) {
                    return player.GetUsername() == username;
                    });

                if (it == lobby.end()) {
                    Player player(username, fireRate, fireRateUpgrades, bulletSpeed, isbulletSpeedUpgraded); // Create a Player instance
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

    ThreadPool threadPool(4);  // Create a thread pool with 4 threads

    CROW_ROUTE(app, "/move").methods(crow::HTTPMethod::POST)([&map, &mapMutex, &threadPool](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);
        size_t newX = x["x"].i();
        size_t newY = x["y"].i();
        size_t playerID = x["playerID"].i();
        if (playerID >= 4) return crow::response(403, "Invalid playerID.");

        // The thread pool enqueues the task
        threadPool.enqueue([&map, &mapMutex, newX, newY, playerID] {
            std::lock_guard<std::mutex> lock(mapMutex);
            // Validate and perform the movement here
            std::pair<size_t, size_t> newPosition(newX, newY);
            if (!map.InBounds(newPosition) || map.GetTile(newPosition) != TileType::EmptySpace) {
                // Handle invalid move
                return;
            }
            auto oldPosition = map.GetPlayerPosition(playerID);
            map.SetTile(oldPosition, TileType::EmptySpace);
            map.SetPlayerPosition(playerID, newPosition);
            map.SetTile(newPosition, TileType::Player);
            });

        // Respond to client with the updated map and player positions
        crow::json::wvalue resultMap;
        resultMap["status"] = "Move successful";

        crow::json::wvalue::list mapArray;
        for (size_t i = 0; i < map.GetHeight(); ++i) {
            crow::json::wvalue::list rowArray;
            for (size_t j = 0; j < map.GetWidth(); ++j) {
                TileType tile = map.GetTile({ i, j });
                rowArray.push_back(static_cast<int>(tile));
            }
            mapArray.push_back(std::move(rowArray));
        }

        resultMap["map"] = std::move(mapArray);

        crow::json::wvalue::list playersArray;
        for (size_t i = 0; i < 4; ++i) { // Assuming 4 players
            auto position = map.GetPlayerPosition(i);
            crow::json::wvalue player;
            player["id"] = i;
            player["x"] = position.first;
            player["y"] = position.second;
            playersArray.push_back(std::move(player));
        }
        resultMap["players"] = std::move(playersArray);

        return crow::response(resultMap);
        });

    CROW_ROUTE(app, "/shoot").methods(crow::HTTPMethod::POST)([&map, &mapMutex, &threadPool, &isShootActive](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);

        size_t playerID = x["playerID"].i();
        std::string direction = x["direction"].s();
        size_t xPos = x["x"].i();
        size_t yPos = x["y"].i();
        size_t speed = x["speed"].i();

        if (playerID >= 4) {
            return crow::response(400, "Invalid player ID");
        }

        Direction shootDirection;
        if (direction == "up") shootDirection = Direction::Up;
        else if (direction == "down") shootDirection = Direction::Down;
        else if (direction == "left") shootDirection = Direction::Left;
        else if (direction == "right") shootDirection = Direction::Right;
        else {
            return crow::response(400, "Invalid direction");
        }

        std::lock_guard<std::mutex> lock(mapMutex);

        std::array<std::shared_ptr<Player>, 4> playersArray;
        for (size_t i = 0; i < 4; ++i) {
            if (i < lobby.size()) {
                playersArray[i] = std::make_shared<Player>(lobby[i]);
            }
            else {
                playersArray[i] = nullptr;
            }
        }

        //BulletManager bulletManager(map, playersArray);
        auto bulletPosition = std::make_pair(xPos, yPos);
        //bulletManager.ShootBullet(bulletPosition, shootDirection, playerID, speed);

        isShootActive.store(true);

        // Return bullet information in the response
        crow::json::wvalue result;
        result["status"] = "Bullet fired";
        result["bullet"] = {
            {"x", xPos},
            {"y", yPos},
            {"direction", direction},
            {"playerID", playerID},
            {"speed", speed}
        };

        return crow::response(result);
        });

    CROW_ROUTE(app, "/bullets")
        .methods(crow::HTTPMethod::POST)([&isShootActive](const crow::request& req) {
        if (!isShootActive.load()) {
            return crow::response(403, "Cannot fetch bullets before shooting.");
        }
        auto x = crow::json::load(req.body);
        if (!x) {
            return crow::response(400);  // Bad request if JSON is invalid
        }

        size_t playerID = x["playerID"].i();
        size_t xPos = x["x"].i();
        size_t yPos = x["y"].i();
        size_t speed = x["speed"].i();
        std::string direction = x["direction"].s();

        if (playerID >= 4) {
            return crow::response(400, "Invalid player ID");
        }

        Direction shootDirection;
        if (direction == "up") shootDirection = Direction::Up;
        else if (direction == "down") shootDirection = Direction::Down;
        else if (direction == "left") shootDirection = Direction::Left;
        else if (direction == "right") shootDirection = Direction::Right;
        else {
            return crow::response(400, "Invalid direction");
        }

        // Create a new bullet (this could be part of a BulletManager)
        Bullet newBullet({ xPos, yPos }, shootDirection, playerID, speed);
        activeBullets.push_back(newBullet);

        // Return response with bullet details
        crow::json::wvalue result;
        result["status"] = "Bullet fired";
        result["bullet"] = { {"x", xPos}, {"y", yPos}, {"direction", direction}, {"playerID", playerID}, {"speed", speed} };

        return crow::response(result);
            });

    app.port(18080).multithreaded().run();
}