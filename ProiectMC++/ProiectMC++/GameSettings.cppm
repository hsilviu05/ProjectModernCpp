export module GameSettings;

import <chrono>;


export class GameSettings{

public:
    static constexpr uint16_t SCORE_PER_ENEMY{ 100 };
    static constexpr uint16_t FIRST_PLACE_POINTS{ 2 };
    static constexpr uint16_t SECOND_PLACE_POINTS { 1 };
    static constexpr uint16_t WIN_BONUS{ 200 };
    static constexpr size_t DEFAULT_BULLET_SPEED{ 2 };
    static constexpr uint16_t MAX_FIRE_RATE_UPGRADES{ 4 };
    static constexpr std::chrono::milliseconds FIRE_RATE_REDUCTION {100};
    static constexpr uint16_t UPGRADE_COST{ 500 };
    static constexpr uint16_t BULLET_SPEED_MULTIPLIER_POINTS{ 10 };
    static constexpr std::chrono::milliseconds COOL_DOWNTIME{800};
private:
    explicit GameSettings()=default;

};
