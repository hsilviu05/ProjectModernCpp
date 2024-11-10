#pragma once

#include "Map.h"
#include "Wall.h"
#include <vector>


enum class Difficulty { EASY, MEDIUM, HARD };

class Level {
public:
    Level(Difficulty difficulty, Map& map);

    int getPlayerHealth() const;
    int getBulletSpeed() const;
    void populateMapWithWalls(Map& map);

private:
    Difficulty difficulty;
    int playerHealth;
    int bulletSpeed;

    void configureDifficulty();
};


