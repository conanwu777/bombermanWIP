#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Position.hpp"
#include "Object.hpp"
#include "Player.hpp"

class Enemy : public Object{
	public:
		Enemy(Pos2D pos, Board& b);
		void update();
		bool	tryMove(int x, int y, char dir, float xOff, float yOff, bool check);
		void onPlayer(Player&);
		void onBomb();
	private:
		char dir;
		float speed;
};

#endif