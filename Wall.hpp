#ifndef WALL_HPP
#define WALL_HPP

#include "Position.hpp"
#include "Object.hpp"

class Wall : public Object{
	public:
		Wall(Pos2D pos, Board& b);
	private:
		
};

#endif