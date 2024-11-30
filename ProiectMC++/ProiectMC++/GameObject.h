 #pragma once
#include <cstdint>
#include <utility>

#include "Direction.h"

class GameObject
{

	public:
	  //virtual void render() = 0;
	  //virtual void update() = 0;
	  std::pair<size_t, size_t> getPosition() const;
	  void setPosition(std::pair<size_t, size_t> t_position);
	  void setSpeed(int16_t t_speed);
	  virtual ~GameObject() = default;
	  GameObject(const std::pair<size_t, size_t>&position,const  size_t& speed,const Direction& direction);
	  GameObject();
	protected:
	  std::pair<size_t, size_t>m_position;
	  int16_t m_speed;
	  Direction m_direction;


	  



		
};

