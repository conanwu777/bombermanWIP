#include "../../inc/game_logic/BreakWall.hpp"
#include "../../inc/game_logic/FirePowerUp.hpp"
#include "../../inc/game_logic/BombPowerUp.hpp"

BreakWall::BreakWall(Pos2D p, Board& b) : Object(BREAKABLE_WALL_GAME, 0, true, true, p, b){

}

void BreakWall::onBomb(){
	board.eraseObject(id);
	int num = rand() % 10;
	if (num < 3){
		board.objs.push_back(new FirePowerUp(pos, board));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
							 board.objs[board.objs.size() - 1]->getType(),
							 board.objs[board.objs.size() - 1]->getPos());
	}
	else if (num < 4){
		board.objs.push_back(new BombPowerUp(pos, board));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
							 board.objs[board.objs.size() - 1]->getType(),
							 board.objs[board.objs.size() - 1]->getPos());
	}
	delete this;
}