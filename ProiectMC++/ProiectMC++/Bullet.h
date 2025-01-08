#pragma once
import GameSettings;
#include "GameObject.h"


class Bullet : public GameObject
{
public:
    Bullet(const std::pair<size_t, size_t>& position, const Direction& direction, const uint8_t shooterID, const size_t speed);
  
    void MoveBullet();


    uint8_t GetShooterID() const;
    void SetShooterID( uint8_t shooterID);

private:
    uint8_t m_shooterID;
};
