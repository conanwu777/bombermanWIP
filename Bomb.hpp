#ifndef BOMB_HPP
#define BOMB_HPP

#include "Position.hpp"
#include "Object.hpp"

class Bomb : public Object{
	public:
		Bomb(Pos2D pos, Board& b);
		void update();
	private:
		float startTime;
		float duration;
		void explode();
};

#endif