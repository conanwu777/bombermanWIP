#include "Player.hpp"

Player::Player(Pos2D p, Board& b) : Object('p', 0, false, true, p, b){
}

bool	Player::tryMove(float x, float y, char dir){
	if (x >= 0 && x < board.bounds.x && y >= 0 && y < board.bounds.y && board.checkWalk(x,y)){
		board.swap(pos.x, pos.y, x, y);
		board.moveDisplay(id, dir, 1);
		pos.x = x;
		pos.y = y;
		board.pX = pos.x;
		board.pY = pos.y;
		return true;
	}
	return false;
}

void Player::move(char dir){
	switch(dir){
		case 'u':
			tryMove(pos.x, pos.y - 1, dir);
			break;
		case 'd':
			tryMove(pos.x, pos.y + 1, dir);
			break;
		case 'l':
			tryMove(pos.x - 1, pos.y, dir);
			break;
		case 'r':
			tryMove(pos.x + 1, pos.y, dir);
			break;
	}
}