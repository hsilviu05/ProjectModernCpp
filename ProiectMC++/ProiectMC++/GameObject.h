#pragma once
#include <cstdint>
#include <utility>

class GameObject
{

	public:
	  virtual void render() = 0;
	  virtual void update() = 0;
	  std::pair<int16_t, int16_t> getPosition() const;
	  void setPostion(std::pair<int16_t, int16_t> t_position);
	  void setSpeed(int16_t t_speed);
		
	protected:
	  std::pair<int16_t, int16_t>m_position;
	  bool m_isActive;
	  int16_t m_speed;
	  int16_t m_direction;


	  



		
};

