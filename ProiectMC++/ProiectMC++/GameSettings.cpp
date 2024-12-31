#include "GameSettings.h"

const int GameSettings::SCORE_PER_ENEMY = 100;
const int GameSettings::FIRST_PLACE_POINTS = 2;
const int GameSettings::SECOND_PLACE_POINTS = 1;
const int GameSettings::WIN_BONUS = 200;
const int GameSettings::MINIM_WALLS_WITH_BOMB = 1;
const int GameSettings::MAXIM_WALLS_WITH_BOMB = 3;
const int GameSettings::DEFAULT_BULLET_SPEED = 1;
const std::chrono::milliseconds GameSettings::COOL_DOWNTIME {500};
const std::chrono::milliseconds GameSettings::FIRE_RATE_REDUCTION {100};


GameSettings::GameSettings() {}
