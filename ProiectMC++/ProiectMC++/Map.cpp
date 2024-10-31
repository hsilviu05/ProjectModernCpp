#include "Map.h"

uint16_t Map::getHeight() const{
	return m_height;
}

void Map::setHeight(uint16_t height){
	m_height = height;
}

uint16_t Map::getWidth() const{
	return m_width;
}

void Map::setWidth(uint16_t width){
	m_width = width;
}
