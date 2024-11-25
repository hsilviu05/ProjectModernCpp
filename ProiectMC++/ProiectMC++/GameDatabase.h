
#pragma once
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING


#include <string>
#include <vector>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;

struct PlayerDB {
    int id;
    std::string name;
    std::string username;
    int points;
    int fire_rate_upgrades;
};

inline auto createStorage(const std::string& filename)
{
    return sql::make_storage(
        filename,
        sql::make_table(
            "Players",
            sql::make_column("id", &PlayerDB::id, sql::primary_key().autoincrement()),
            sql::make_column("name", &PlayerDB::name),
            sql::make_column("username", &PlayerDB::username),
            sql::make_column("points", &PlayerDB::points),
            sql::make_column("fire_rate_upgrades", &PlayerDB::fire_rate_upgrades)));
}
using Storage = decltype(createStorage(""));
