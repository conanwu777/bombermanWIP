#include "Bomb.hpp"
#include "Time.hpp"

Bomb::Bomb(Pos2D p, Board& b) : Object('*', 0, true, true, p, b){
	startTime = Time::curTime;
	duration = 1;
}

void	Bomb::update()
{
	if (Time::curTime > startTime + duration)
		explode();
}

void	Bomb::explode()
{
	board.eraseObject(id);
	//explosion
	delete this;
}