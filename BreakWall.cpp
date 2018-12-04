#include "BreakWall.hpp"

BreakWall::BreakWall(Pos2D p, Board& b) : Object('=', 0, true, true, p, b){

}

void BreakWall::onBomb(){
	board.eraseObject(id);
	delete this;
}