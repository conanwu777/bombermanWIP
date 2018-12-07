#include "PierceBomb.hpp"
#include "Fire.hpp"
#include "Time.hpp"

PierceBomb::PierceBomb(Pos2D p, Board& b, int r, Player& pl) : player(pl), Object(PIERCE_BOMB_GAME, 0, true, true, p, b){
	startTime = Time::curTime;
	duration = 3;
	range = r;
}

void	PierceBomb::update()
{
	if (Time::curTime > startTime + duration)
		explode();
}

void	PierceBomb::onBomb(){
	explode();
}

void	PierceBomb::explode()
{
	player.addBomb();
	board.eraseObject(id);
	board.objs.push_back(new Fire(pos, board, range, 'a', true));
	board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
		board.objs[board.objs.size() - 1]->getType(),
		board.objs[board.objs.size() - 1]->getPos());
	delete this;
}