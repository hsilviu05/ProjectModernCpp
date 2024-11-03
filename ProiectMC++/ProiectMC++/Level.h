#pragma once

#ifndef LEVEL_H
#define LEVEL_H

enum class Difficulty { EASY, MEDIUM, HARD };

class Level {
public:
    Level(Difficulty difficulty);

    int getPlayerHealth() const;
    int getBulletSpeed() const;

private:
    Difficulty difficulty;
    int playerHealth;
    int bulletSpeed;

    void configureDifficulty();
};

#endif // LEVEL_H

