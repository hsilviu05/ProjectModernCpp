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


                const auto& lobbyPlayers = playerManager.GetLobbyPlayersConst();
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



    CROW_ROUTE(app, "/join_game").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid request body");


        std::string username = body["username"].s();
        std::cout << "Received join_game request for username: " << username << std::endl;

        if (username.empty()) {
            std::cout << "Username is empty" << std::endl;
            return crow::response(400, "Missing username in request body");
        }

        std::unique_lock<std::mutex> lock(lobbyMutex);

        auto& lobbyPlayers = playerManager.GetLobbyPlayers();
        auto it = std::find_if(lobbyPlayers.begin(), lobbyPlayers.end(), [&username](const std::shared_ptr<Player>& player) {
            return player->GetUsername() == username;
            });

        if (it == lobbyPlayers.end()) {
            std::cout << "Player not found in lobby: " << username << std::endl;
            return crow::response(404, "Player not found in lobby");
        }

        auto& activePlayers = playerManager.GetActivePlayers();
        if (activePlayers.size() < 4) {
            for (size_t i = 0; i < activePlayers.size(); ++i) {
                if (!activePlayers[i]) {
                    auto player = std::move(*it);
                    it = lobbyPlayers.erase(it);
                    activePlayers[i] = std::move(player);
                    std::cout << "Player " << username << " added to active players.\n";
                    break;
                }
            }
        }
        else {
            return crow::response(400, "Game is already full");
        }

        if (std::count_if(activePlayers.begin(), activePlayers.end(), [](const std::shared_ptr<Player>& player) {
            return player != nullptr;
            }) >= 2) {
            static bool gameStarting = false;
            if (!gameStarting) {
                gameStarting = true;
                StartGame();
            }
        }
    });
    	



    CROW_ROUTE(app, "/get_current_score").methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
        auto username = req.url_params.get("username");
        if (!username) {
            return crow::response(400, "Missing username parameter");
        }

        std::string userStr = username;


        const auto& players = game->GetPlayers(); // Metodă pentru a accesa m_players
        for (const auto& player : players) {
            if (player && player->GetUsername() == userStr) {
                crow::json::wvalue result;
                result["username"] = player->GetUsername();
                result["current_score"] = player->GetScore(); // Scorul curent al jucătorului
                return crow::response(result);
            }
        }

        return crow::response(404, "Player not found in current game");
        });


    CROW_ROUTE(app, "/get_total_score_and_points").methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
        auto username = req.url_params.get("username");
        if (!username) {
            return crow::response(400, "Missing username parameter");
        }

        std::string userStr = username;
        std::string dbFile = "account_data.db";

        try {
            accountManager.LoadDataFromDatabase(dbFile, userStr);

            uint16_t totalScore = accountManager.GetScore();
            uint16_t totalPoints = accountManager.GetPoints();

            // Returnează scorul în format JSON
            crow::json::wvalue result;
            result["username"] = userStr;
            result["total_score"] = totalScore;
            result["total_points"] = totalPoints;
            return crow::response(result);
        }
        catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });




	app.port(8080).multithreaded().run();
}

	

void Routing::StartGame()
{

    static bool gameStarting = false;

    if (gameStarting) {
        std::cout << "A game is already starting.\n";
        return;
    }

    gameStarting = true;

    std::thread([this]() {
        std::this_thread::sleep_for(std::chrono::seconds(30)); // Așteptăm 30 de secunde pentru mai mulți jucători

        {
            std::unique_lock<std::mutex> lock(lobbyMutex); // Blocăm accesul la lobby pentru verificare
            auto& activePlayers = playerManager.GetActivePlayers();

            // Re-verificăm numărul de jucători la expirarea timer-ului
            size_t activeCount = std::count_if(activePlayers.begin(), activePlayers.end(), [](const std::shared_ptr<Player>& player) {
                return player != nullptr;
                });

            if (activeCount >= 2) {
                std::cout << "Starting game with " << activeCount << " players.\n";

                // Mutăm logica de creare a jocului aici
                game = std::make_unique<Game>(playerManager);
                game->Start(); // Pornește bucla principală a jocului
            }
            else {
                std::cout << "Not enough players to start the game. Resetting...\n";
            }
        }

        gameStarting = false;
        }).detach();
}

void Routing::ResetGame()
{
    if (game) {
        game.reset();
    }
}
