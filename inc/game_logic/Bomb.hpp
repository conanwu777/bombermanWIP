#ifndef BOMB_HPP
#define BOMB_HPP

#include "Position.hpp"
#include "Object.hpp"
#include "Player.hpp"

class Bomb : public Object{
	public:
		Bomb(Pos2D pos, Board& b, int , Player&);
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