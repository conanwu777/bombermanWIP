#include "../../inc/game_logic/Player.hpp"
#include "../../inc/game_logic/Bomb.hpp"

Player::Player(Pos2D p, Board& b) : Object('o', 0, true, false, p, b){
}

bool	Player::tryMove(float x, float y, char dir, float xOff, float yOff, bool check){
	if (check || (x >= 0 && x < board.bounds.x && y >= 0 && y < board.bounds.y && board.checkEmpty(x,y))){
		// board.swap(pos.x, pos.y, x, y);
		if (dir == 'r' || dir == 'l'){
			if (fabs(off.y) > 0.25f)
				return false;
			else{
//				board.moveDisplay(id, dir, 0.2);
//				off.y = 0;
			}
		}

		if (dir == 'u' || dir == 'd'){
			if (fabs(off.x) > 0.25f)
				return false;
			else{
//				board.moveDisplay(id, dir, 0.2);
//				off.x = 0;
			}
		}


		off.x += xOff;
		off.y += yOff;
		board.moveDisplay(id, dir, 0.2);
		if (off.x > 0.5){
			off.x = -1 + off.x;
			pos.x++;
		}
		else if(off.x < -0.5){
			off.x = 1 + off.x;
			pos.x--;
		}

		if (off.y > 0.5){
			off.y = -1 + off.y;
			pos.y++;
		}
		else if(off.y < -0.5){
			off.y = 1 + off.y;
			pos.y--;
		}


		return true;
	}
	return false;
}

void Player::dropBomb(){
	if (board.checkEmpty(pos.x, pos.y)){
		board.objs.push_back(new Bomb(pos, board));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
							 board.objs[board.objs.size() - 1]->getType(),
							 board.objs[board.objs.size() - 1]->getPos());
	}
}
void Player::onBomb(){
	// cout << "You've DIEDED \n";
}

void Player::move(char dir){
	switch(dir){
		case 'u':
			if (off.y < 0)
				tryMove(pos.x, pos.y - 1, dir, 0, -0.2f, false);
			else
				tryMove(pos.x, pos.y - 1, dir, 0, -0.2f, true);
			break;
		case 'd':
			if (off.y > 0)
				tryMove(pos.x, pos.y + 1, dir, 0, 0.2f, false);
			else
				tryMove(pos.x, pos.y + 1, dir, 0, 0.2f, true);
			break;
		case 'l':
			if (off.x < 0)
				tryMove(pos.x - 1, pos.y, dir, -0.2f, 0, false);
			else
				tryMove(pos.x - 1, pos.y, dir, -0.2f, 0, true);
			break;
		case 'r':
			if (off.x > 0)
				tryMove(pos.x + 1, pos.y, dir, 0.2f, 0, false);
			else
				tryMove(pos.x + 1, pos.y, dir, 0.2f, 0, true);
			break;
	}
}