#include "BreakWall.hpp"
#include "FirePowerUp.hpp"
#include "PiercePowerUp.hpp"
#include "SkatesPowerUp.hpp"
#include "BombPowerUp.hpp"

BreakWall::BreakWall(Pos2D p, Board& b) : Object(BREAKABLE_WALL_GAME, 0, true, true, p, b){

}

void BreakWall::onBomb(){
	board.eraseObject(id);
	int num = rand() % 50;
	if (num < 10){
		board.objs.push_back(new FirePowerUp(pos, board));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
							 board.objs[board.objs.size() - 1]->getType(),
							 board.objs[board.objs.size() - 1]->getPos());
	}
	else if (num < 20){
		board.objs.push_back(new BombPowerUp(pos, board));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
							 board.objs[board.objs.size() - 1]->getType(),
							 board.objs[board.objs.size() - 1]->getPos());
	}
	else if (num < 20){
		board.objs.push_back(new BombPowerUp(pos, board));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
							 board.objs[board.objs.size() - 1]->getType(),
							 board.objs[board.objs.size() - 1]->getPos());
	}
	else if (num < 30){
		board.objs.push_back(new SkatesPowerUp(pos, board));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
							 board.objs[board.objs.size() - 1]->getType(),
							 board.objs[board.objs.size() - 1]->getPos());
	}
	else if (num < 32){
		board.objs.push_back(new PiercePowerUp(pos, board));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
							 board.objs[board.objs.size() - 1]->getType(),
							 board.objs[board.objs.size() - 1]->getPos());
	}
	delete this;
}