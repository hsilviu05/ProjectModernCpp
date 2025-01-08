 #pragma once


#include<utility>
#include "Direction.h"

class GameObject
{

	public:
	  std::pair<size_t, size_t> getPosition() const;
	  void SetPosition(const std::pair<size_t, size_t>& position);

	  size_t GetSpeed() const;
	  void SetSpeed(const size_t& speed);

	  Direction GetDirection() const;
	  void SetDirection(const Direction& direction);
	  
	  virtual ~GameObject() = default;
	  GameObject(const std::pair<size_t, size_t>&position,const  size_t& speed,const Direction& direction);
	  GameObject();
	protected:
	  std::pair<size_t, size_t>m_position;
	  size_t m_speed;
	  Direction m_direction;


	  



		
};

