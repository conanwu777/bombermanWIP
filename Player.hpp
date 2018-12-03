#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Position.hpp"
#include "Object.hpp"

class Player : public Object{
	public:
		Player(Pos2D pos, Board& b);
		bool tryMove(float x, float y, char dir);
		void move(char dir);
	private:
		
};

#endif