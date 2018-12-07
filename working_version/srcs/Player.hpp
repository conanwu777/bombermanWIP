#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Position.hpp"
#include "Object.hpp"

class Player : public Object{
	public:
		Player(Pos2D pos, Board& b);
		bool tryMove(int x, int y, char dir, float xOff, float yOff, bool check);
		void dropBomb();
		void move(char dir);
		void addBomb();
		void incRange(int range);
		void onBomb();
	private:
		int numBombs;
		int bombRange;
		float speed;
};

#endif