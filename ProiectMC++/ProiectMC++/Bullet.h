#pragma once
#include "Direction.h"
#include "GameObject.h"

class Bullet : public GameObject
{
public:
    Bullet(const std::pair<size_t, size_t>& position, const Direction& direction, const size_t& shooterID, const size_t& speed);
  
    void MoveBullet();

    Direction GetDirection();
    void SetDirection(const Direction& direction);

    uint8_t GetShooterID() const;
    void SetShooterID(const uint8_t& shooterID);

private:
    uint8_t m_shooterID;
};
