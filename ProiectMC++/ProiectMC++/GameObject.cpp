#include "GameObject.h"

std::pair<size_t, size_t> GameObject::getPosition() const{
	return m_position;
}

void GameObject::setPosition(std::pair<size_t, size_t> t_position){
	m_position = t_position;
}

void GameObject::setSpeed(int16_t t_speed){
	m_speed = t_speed;
}

GameObject::GameObject(const std::pair<size_t, size_t>& position,const size_t& speed,const Direction& direction):m_position(position),m_speed(speed),m_direction(direction)
{

}

GameObject::GameObject()
{

}
