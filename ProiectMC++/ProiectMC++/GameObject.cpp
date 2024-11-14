#include "GameObject.h"

std::pair<size_t, size_t> GameObject::getPosition() const{
	return m_position;
}

void GameObject::setPostion(std::pair<int16_t, int16_t> t_position){
	m_position = t_position;
}

void GameObject::setSpeed(int16_t t_speed){
	m_speed = t_speed;
}
