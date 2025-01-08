#include "GameObject.h"

std::pair<size_t, size_t> GameObject::getPosition() const{
	return m_position;
}

void GameObject::SetPosition(const std::pair<size_t, size_t> &position){
	m_position = position;
}

void GameObject::SetSpeed(const size_t& speed){
	m_speed = speed;
}

Direction GameObject::GetDirection() const
{
	return m_direction;
}

void GameObject::SetDirection(const Direction& direction)
{
	m_direction = direction;
}

size_t GameObject::GetSpeed() const
{
	return m_speed;
}

GameObject::GameObject(const std::pair<size_t, size_t>& position,const size_t& speed,const Direction& direction):m_position(position),m_speed(speed),m_direction(direction)
{

}

GameObject::GameObject()
{

}
