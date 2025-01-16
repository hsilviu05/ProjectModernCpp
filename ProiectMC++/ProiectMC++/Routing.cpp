#include "Routing.h"

void Routing::run(Game &game)
{
    CROW_ROUTE(app, "/map")
        .methods("GET"_method)
        ([&game,this]() {
        std::lock_guard<std::mutex> lock(mapMutex);
        const auto& map = game.GetMap();

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

/*
    CROW_ROUTE(app, "/input")
        .methods("POST"_method)
        ([&game](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("input")) {
            return crow::response(400, "Invalid input format");
        }

        std::string username = body["username"].s();
        char input = body["input"].s()[0];

        game.receiveInput(username, input); 

        return crow::response(200, "Input received");
            });
*/
}
