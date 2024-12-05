#pragma once
class GameSettings
{
public:
    static const int POINTS_PER_ENEMY;
    static const int WIN_BONUS;
    static const int MINIM_WALLS_WITH_BOMB;
    static const int MAXIM_WALLS_WITH_BOMB;
    static constexpr int MAX_FIRE_RATE_UPGRADES = 4;
    static constexpr int FIRE_RATE_REDUCTION = 100;
    static constexpr int UPGRADE_COST = 500;
    static constexpr int BULLET_SPEED_MULTIPLIER_POINTS = 10;
    static constexpr int COOL_DOWNTIME = 500;

private:
    explicit GameSettings();
};

