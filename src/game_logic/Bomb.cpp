#include "../../inc/game_logic/Bomb.hpp"
#include "../../inc/game_logic/Fire.hpp"
#include "../../inc/game_logic/Time.hpp"

Bomb::Bomb(Pos2D p, Board& b) : Object('*', 0, true, true, p, b){
	startTime = Time::curTime;
	duration = 1;
}

void	Bomb::update()
{
	if (Time::curTime > startTime + duration)
		explode();
}

void	Bomb::onBomb(){
	explode();
}

void	Bomb::explode()
{
	board.eraseObject(id);
	board.objs.push_back(new Fire(pos, board, 3, 'a'));
	board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
		board.objs[board.objs.size() - 1]->getType(),
		board.objs[board.objs.size() - 1]->getPos());
	delete this;
}