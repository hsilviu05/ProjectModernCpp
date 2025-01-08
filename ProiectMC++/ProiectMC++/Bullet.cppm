export module Bullet;

#include "GameObject.h"
#include "GameSettings.h"
#include "Direction.h" 

export class Bullet : public GameObject{
public:
	Bullet(const std::pair<size_t, size_t>& position, const Direction& direction, const size_t& shooterID, const size_t& speed = GameSettings::DEFAULT_BULLET_SPEED)
		: GameObject(position, speed, direction), m_shooterID(shooterID) {}
	void MoveBullet()
	{
		switch (m_direction) {
			case Direction::Up:m_position.first--; break;
			case Direction::Down:m_position.first++; break;
			case Direction::Left:m_position.second--; break;
			case Direction::Right:m_position.second++; break;
		}

	}
	Direction GetDirection() const { return m_direction; }
	void SetDirection( Direction direction) { m_direction = direction; }

	uint8_t GetShooterID() const { return m_shooterID; }
	void SetShooterID(const uint8_t& shooterID) { m_shooterID = shooterID; }

private:
	uint8_t m_shooterID;
};

