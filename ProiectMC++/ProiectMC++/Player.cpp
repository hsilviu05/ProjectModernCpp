//#include "Player.h"
//#include"Level.h"
//
//Player::Player(int startX, int startY, int health)
//    : m_x(startX), m_y(startY),health(health) {}
//
//void Player::move(Direction dir) {
//    direction = dir;
//    switch (dir) {
//    case Direction::Up:    m_y -= 1; break;
//    case Direction::Down:  m_y += 1; break;
//    case Direction::Left:  m_x -= 1; break;
//    case Direction::Right: m_x += 1; break;
//    }
//}
//
//
//void Player::takeDamage() {
//    if (health > 0) {
//        health--;
//    }
//}
//
//bool Player::isAlive() const {
//    return health > 0;
//}
//
//bool Player::isEliminated() const {
//    return health <= 0;
//}
//
//void Player::respawn() {
//    m_x = initialX;
//    m_y = initialY;
//    health = 3;
//}
//
//int Player::getX() const { return m_x; }
//
//int Player::getY() const { return m_y; }
//
//int Player::getHealth() const {
//    return health;
//}
//
