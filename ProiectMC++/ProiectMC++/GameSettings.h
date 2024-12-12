#pragma once
#include <chrono>
class GameSettings
{
public:
    static const int POINTS_PER_ENEMY;
    static const int WIN_BONUS;
    static const int MINIM_WALLS_WITH_BOMB;
    static const int MAXIM_WALLS_WITH_BOMB;
    static const int DEFAULT_BULLET_SPEED;
    static constexpr int MAX_FIRE_RATE_UPGRADES = 4;
    static const std::chrono::milliseconds FIRE_RATE_REDUCTION;
    static constexpr int UPGRADE_COST = 500;
    static constexpr int BULLET_SPEED_MULTIPLIER_POINTS = 10;
    static const std::chrono::milliseconds COOL_DOWNTIME;

private:
    explicit GameSettings();
};

