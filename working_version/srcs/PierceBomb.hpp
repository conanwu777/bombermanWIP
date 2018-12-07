#ifndef PBOMB_HPP
#define PBOMB_HPP

#include "Position.hpp"
#include "Object.hpp"
#include "Player.hpp"

class PierceBomb : public Object{
	public:
		PierceBomb(Pos2D pos, Board& b, int , Player&);
		void onBomb();
		void update();
	private:
		float startTime;
		Player& player;
		float duration;
		int range;
		void explode();

};

#endif