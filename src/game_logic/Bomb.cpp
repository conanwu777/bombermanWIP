#include "../../inc/game_logic/Bomb.hpp"
#include "../../inc/game_logic/Fire.hpp"
#include "../../inc/game_logic/Time.hpp"

Bomb::Bomb(Pos2D p, Board& b, int r, Player& pl) : player(pl), Object(BOMB_GAME, 0, true, true, p, b){
	startTime = Time::curTime;
	duration = 3;
	range = r;
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
	player.addBomb();
	board.eraseObject(id);
	board.objs.push_back(new Fire(pos, board, range, 'a'));
	board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
		board.objs[board.objs.size() - 1]->getType(),
		board.objs[board.objs.size() - 1]->getPos());
	delete this;
}