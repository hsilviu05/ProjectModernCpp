#include "Level.h"

Level::Level(Difficulty difficulty, Map& map)
    : difficulty(difficulty) {
    configureDifficulty();
    populateMapWithWalls(map); 
}

void Level::configureDifficulty() {

    switch (difficulty) {
    case Difficulty::EASY:
        playerHealth = 3;
        bulletSpeed = 1;
        break;
    case Difficulty::MEDIUM:
        playerHealth = 3;
        bulletSpeed = 2;
        break;
    case Difficulty::HARD:
        playerHealth = 1;
        bulletSpeed = 3;
        break;
    }
}

int Level::getPlayerHealth() const {
    return playerHealth;
}

int Level::getBulletSpeed() const {
    return bulletSpeed;
}

void Level::populateMapWithWalls(Map& map) {
    srand(time(0));

    int totalWalls = 0;
    int bombWalls = 0;

    switch (difficulty) {
    case Difficulty::EASY:
        totalWalls = 5;
        bombWalls = 1;
        break;
    case Difficulty::MEDIUM:
        totalWalls = 10;
        bombWalls = 3;
        break;
    case Difficulty::HARD:
        totalWalls = 20;
        bombWalls = 7;
        break;
    }
}