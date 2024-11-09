#include "Level.h"

Level::Level(Difficulty difficulty) : difficulty(difficulty) {
    configureDifficulty();
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
