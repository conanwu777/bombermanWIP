#ifndef BREAKWALL_HPP
#define BREAKWALL_HPP

#include "Position.hpp"
#include "Object.hpp"

class BreakWall : public Object{
	public:
		BreakWall(Pos2D pos, Board& b);
		void onBomb();
	private:
		
};

#endif