#include "Player.hpp"
#include "Bomb.hpp"

Player::Player(Pos2D p, Board& b) : Object('o', 0, true, false, p, b){
}

bool	Player::tryMove(float x, float y, char dir, float xOff, float yOff, bool check){
	if (x >= 0 && x < board.bounds.x && y >= 0 && y < board.bounds.y && board.checkEmpty(x,y)){
		// board.swap(pos.x, pos.y, x, y);
		board.moveDisplay(id, dir, 1);
		pos.x = x;
		pos.y = y;
		return true;
	}
	return false;
}

void Player::dropBomb(){
	board.objs.push_back(new Bomb(pos, board));
	board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
		board.objs[board.objs.size() - 1]->getType(),
		board.objs[board.objs.size() - 1]->getPos());
}

void Player::onBomb(){
	cout << "You've DIEDED \n";
}

void Player::move(char dir){
	switch(dir){
		case 'u':
			if (off.y < 0)
				tryMove(pos.x, pos.y - 1, dir, 0, -0.01f, false);
			else
				tryMove(pos.x, pos.y - 1, dir, 0, -0.01f, true);
			break;
		case 'd':
			if (off.y > 0)
				tryMove(pos.x, pos.y + 1, dir, 0, 0.01f, false);
			else
				tryMove(pos.x, pos.y + 1, dir, 0, 0.01f, true);
			break;
		case 'l':
			if (off.x < 0)
				tryMove(pos.x - 1, pos.y, dir, -0.01, 0, false);
			else
				tryMove(pos.x - 1, pos.y, dir, -0.01, 0, true);
			break;
		case 'r':
			if (off.x < 0)
				tryMove(pos.x + 1, pos.y, dir, 0.01, 0, false);
			else
				tryMove(pos.x + 1, pos.y, dir, 0.01, 0, true);
			break;
	}
}