#include "Bomb.hpp"
#include "Time.hpp"

Bomb::Bomb(Pos2D p, Board& b) : Object('p', 0, true, true, p, b){
	startTime = Time::curTime;
}

void	Bomb::update()
{
	if (Time::curTime > startTime + duration)
		explode();
}

void	Bomb::explode()
{
	board.board[pos.x][pos.y] = NULL;
	board.display.eraseObj(id);
	//explosion
	delete this;
}