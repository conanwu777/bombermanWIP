#include "Player.hpp"
#include "Bomb.hpp"
#include "PierceBomb.hpp"
#include <math.h>

Player::Player(Pos2D p, Board& b) : Object(PLAYER_GAME, 0, true, false, p, b){
	numBombs = 1;
	bombRange = 1;
	speed = 0.05;
	pierce = false;
}

bool	Player::tryMove(int x, int y, char dir, float xOff, float yOff, bool check){
	if (check || (x >= 0 && x < board.bounds.x && y >= 0 && y < board.bounds.y && board.checkEmpty(x,y))){
		// board.swap(pos.x, pos.y, x, y);
		if (dir == 'r' || dir == 'l'){
			if (fabs(off.y) > 0.35f)
				return false;
			else{
//				board.moveDisplay(id, dir, 0.2);
				off.y = 0;
			}
		}

		if (dir == 'u' || dir == 'd'){
			if (fabs(off.x) > 0.35f)
				return false;
			else{
//				board.moveDisplay(id, dir, 0.2);
				off.x = 0;
			}
		}


		off.x += xOff;
		off.y += yOff;
		if (off.x > 0.5){
			off.x = -1 + off.x;
			pos.x++;
			board.playerMoved(pos.x, pos.y, *this);
		}
		else if(off.x < -0.5){
			off.x = 1 + off.x;
			pos.x--;
			board.playerMoved(pos.x, pos.y, *this);
		}

		if (off.y > 0.5){
			off.y = -1 + off.y;
			pos.y++;
			board.playerMoved(pos.x, pos.y, *this);
		}
		else if(off.y < -0.5){
			off.y = 1 + off.y;
			pos.y--;
			board.playerMoved(pos.x, pos.y, *this);
		}

		board.moveDisplay(id, dir, pos.x + off.x, pos.y + off.y);

		return true;
	}
	return false;
}

void Player::dropBomb(){
	if (numBombs > 0 && board.checkEmpty(pos.x, pos.y)){
		numBombs--;
		if (pierce){
			board.objs.push_back(new PierceBomb(pos, board, bombRange, *this));
		}else{
			board.objs.push_back(new Bomb(pos, board, bombRange, *this));
		}
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
							 board.objs[board.objs.size() - 1]->getType(),
							 board.objs[board.objs.size() - 1]->getPos());
	}
}
void Player::onBomb(){
	cout << "You've DIEDED \n";
}

void Player::addBomb(){
	numBombs++;
}

void Player::speedUp(){
	speed += 0.01f;
}


void Player::activatePierce(){
	pierce = true;
}

void Player::incRange(int num){
	bombRange += num;
}

void Player::move(char dir){
	switch(dir){
		case 'u':
			if (off.y < speed)
				tryMove(pos.x, pos.y - 1, dir, 0, -speed, false);
			else
				tryMove(pos.x, pos.y - 1, dir, 0, -speed, true);
			break;
		case 'd':
			if (off.y > -speed)
				tryMove(pos.x, pos.y + 1, dir, 0, speed, false);
			else
				tryMove(pos.x, pos.y + 1, dir, 0, speed, true);
			break;
		case 'l':
			if (off.x < speed)
				tryMove(pos.x - 1, pos.y, dir, -speed, 0, false);
			else
				tryMove(pos.x - 1, pos.y, dir, -speed, 0, true);
			break;
		case 'r':
			if (off.x > -speed)
				tryMove(pos.x + 1, pos.y, dir, speed, 0, false);
			else
				tryMove(pos.x + 1, pos.y, dir, speed, 0, true);
			break;
	}
}