#include "Enemy.hpp"

Enemy::Enemy(Pos2D pos, Board& b) : Object(ENEMY_GAME, 1, true, false, pos, b){
	dir = 'd';
	speed = 0.02f;
}

bool	Enemy::tryMove(int x, int y, char dir, float xOff, float yOff, bool check){
	if (check || (x >= 0 && x < board.bounds.x && y >= 0 && y < board.bounds.y && board.checkEmpty(x,y))){
		// board.swap(pos.x, pos.y, x, y);
		if (dir == 'r' || dir == 'l'){
			off.y = 0;
		}

		if (dir == 'u' || dir == 'd'){
			off.x = 0;
		}


		off.x += xOff;
		off.y += yOff;
		board.moveDisplay(id, dir, pos.x + off.x, pos.y + off.y);
		
		if (off.x > 0.5){
			off.x = -1 + off.x;
			pos.x++;

			board.enemyMoved(pos.x, pos.y, *this);
		}
		else if(off.x < -0.5){
			off.x = 1 + off.x;
			pos.x--;
			board.enemyMoved(pos.x, pos.y, *this);
		}

		if (off.y > 0.5){
			off.y = -1 + off.y;
			pos.y++;
			board.enemyMoved(pos.x, pos.y, *this);
		}
		else if(off.y < -0.5){
			off.y = 1 + off.y;
			pos.y--;
			board.enemyMoved(pos.x, pos.y, *this);
		}

		return true;
	}
	return false;
}
	
void Enemy::update(){
	bool moveSucces;
	switch(dir){
		case 'u':
			if (off.y < speed)
				moveSucces = tryMove(pos.x, pos.y - 1, dir, 0, -speed, false);
			else
				moveSucces = tryMove(pos.x, pos.y - 1, dir, 0, -speed, true);
			break;
		case 'd':
			if (off.y > -speed)
				moveSucces = tryMove(pos.x, pos.y + 1, dir, 0, speed, false);
			else
				moveSucces = tryMove(pos.x, pos.y + 1, dir, 0, speed, true);
			break;
		case 'l':
			if (off.x < speed)
				moveSucces = tryMove(pos.x - 1, pos.y, dir, -speed, 0, false);
			else
				moveSucces = tryMove(pos.x - 1, pos.y, dir, -speed, 0, true);
			break;
		case 'r':
			if (off.x > -speed)
				moveSucces = tryMove(pos.x + 1, pos.y, dir, speed, 0, false);
			else
				moveSucces = tryMove(pos.x + 1, pos.y, dir, speed, 0, true);
			break;
	}
	if (!moveSucces){
		int num = rand() % 4;
		switch (num){
			case 0:
				dir = 'd';
				break;
			case 1:
				dir = 'u';
				break;
			case 2:
				dir = 'r';
				break;
			case 3:
				dir = 'l';
				break;
		}
	}
}

void Enemy::onPlayer(Player& pl){
	pl.onBomb();
}

void Enemy::onBomb(){
	board.eraseObject(id);
	delete this;
}