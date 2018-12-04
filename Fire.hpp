#ifndef FIRE_HPP
#define FIRE_HPP

#include "Position.hpp"
#include "Object.hpp"

class Fire : public Object{
	public:
		Fire(Pos2D pos, Board& b, int range, char dir);
		void update();
	private:
		float startTime;
		float duration;
};

#endif