#ifndef FIRE_HPP
#define FIRE_HPP

#include "Position.hpp"
#include "Object.hpp"
#include "Player.hpp"

class Fire : public Object{
	public:
		Fire(Pos2D pos, Board& b, int range, char dir, bool);
		void update();
		void onPlayer(Player&);
	private:
		float startTime;
		float duration;
};

#endif