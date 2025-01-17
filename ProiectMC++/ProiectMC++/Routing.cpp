#include "Routing.h"

void Routing::Run()
{
    crow::SimpleApp app;
    CROW_ROUTE(app, "/map")
        .methods(crow::HTTPMethod::GET)
        ([this]() {
        std::lock_guard<std::mutex> lock(mapMutex);
        const auto& map = this->game->GetMap();

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


    CROW_ROUTE(app, "/move")
.methods(crow::HTTPMethod::POST)
([this](const crow::request& req) {
    auto body = crow::json::load(req.body);
    if (!body || !body.has("username") || !body.has("input")) {
        return crow::response(400, "Invalid input format");
    }

    std::string username = body["username"].s();
	std::string input = body["input"].s();

    // Verifică dacă inputul este valid pentru mișcare
    if (input[0] != 'W' && input[0] != 'A' && input[0] != 'S' && input[0] != 'D') {
        return crow::response(400, "Invalid move input");
    }

    this->game->ReceiveInput(username, input[0]);

    return crow::response(200, "Move input received");
});

    CROW_ROUTE(app, "/shoot")
        .methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username")) {
            return crow::response(400, "Invalid input format");
        }

        std::string username = body["username"].s();

        char input = ' ';

        this->game->ReceiveInput(username, input);

        return crow::response(200, "Shoot input received");
            });

    CROW_ROUTE(app, "/signup").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400);

        std::string username = body["username"].s();
        std::string password = body["password"].s();
        std::string dbFile = "account_data.db";

        try {
            this->accountManager.SignUp(dbFile, username, password);
            return crow::response(200, "Account created successfully!");
        }
        catch (const std::exception& e) {
            return crow::response(403, e.what());
        }
        });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400);

        std::string username = body["username"].s();
        std::string password = body["password"].s();
        std::string dbFile = "account_data.db";

        try {
            accountManager.LoadDataFromDatabase(dbFile, username);

            if (accountManager.Authenticate(username, password)) {
                uint16_t points = accountManager.GetPoints();

                std::chrono::milliseconds fireRate(accountManager.GetFireRate());
                uint8_t fireRateUpgrades = GameSettings::MAX_FIRE_RATE_UPGRADES;
                double bulletSpeed = GameSettings::DEFAULT_BULLET_SPEED;
                bool bulletSpeedUpgraded = accountManager.GetSpeedBoost();


                const auto& lobbyPlayers = playerManager.GetLobbyPlayers();
                auto it = std::find_if(lobbyPlayers.begin(), lobbyPlayers.end(), [&username](const std::shared_ptr<Player>& player) {
                    return player->GetUsername() == username;
                    });

                if (it != lobbyPlayers.end()) {
                    return crow::response(403, "Player is already logged in and in the lobby.");
                }

                auto player = std::make_shared<Player>(username, fireRate, fireRateUpgrades, bulletSpeed, bulletSpeedUpgraded);

                playerManager.AddToLobby(player);

                return crow::response(200, "Login successful!");
            }else {
                return crow::response(403, "Invalid credentials.");
            }
        }
        catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });

    app.port(8080).multithreaded().run();
	

}

void Routing::StartGame()
{
    if (game) {
        ResetGame();
    }
    //game = std::make_unique<Game>();
}

void Routing::ResetGame()
{
    if (game) {
        game.reset();
    }
}
