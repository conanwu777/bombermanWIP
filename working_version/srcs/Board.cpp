#include "Board.hpp"
#include "Wall.hpp"
#include "BreakWall.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

void Board::initBoard(){

	Pos2D tmp;

	objs.reserve(bounds.x * bounds.y * sizeof(Object*));
	
	tmp.x = 1;
	tmp.y = 1;
	objs.push_back(new Player(tmp, *this));
	display.addObj(objs[objs.size() - 1]->getId(),
		objs[objs.size() - 1]->getType(), objs[objs.size() - 1]->getPos());

	tmp.x = 3;
	tmp.y = 4;
	objs.push_back(new Enemy(tmp, *this));
	display.addObj(objs[objs.size() - 1]->getId(),
		objs[objs.size() - 1]->getType(), objs[objs.size() - 1]->getPos());

	tmp.x = 6;
	tmp.y = 5;
	objs.push_back(new Enemy(tmp, *this));
	display.addObj(objs[objs.size() - 1]->getId(),
		objs[objs.size() - 1]->getType(), objs[objs.size() - 1]->getPos());

	tmp.x = 5;
	tmp.y = 8;
	objs.push_back(new Enemy(tmp, *this));
	display.addObj(objs[objs.size() - 1]->getId(),
		objs[objs.size() - 1]->getType(), objs[objs.size() - 1]->getPos());

	for (int x = 0; x < bounds.x; x++)
	{
		tmp.x = x;
		for (int y = 0; y < bounds.y; y++)
		{
			tmp.y = y;
			if (x == 0 || y == 0 || x == bounds.x - 1 || y == bounds.y - 1
				|| (x % 2 == 0 && y % 2 == 0))
			{
				objs.push_back(new Wall(tmp, *this));
				display.addObj(objs[objs.size() - 1]->getId(),
					objs[objs.size() - 1]->getType(), objs[objs.size() - 1]->getPos());
			}
			else if(x + y > 4){
				if ((x != 3 || y != 4) && (x != 3 || y != 3) && (x != 6 || y != 5) && (x != 9 || y != 9) && (x != 7 || y != 5) && (x != 5 || y != 8) && (x != 5 || y != 9)){
					if (rand() % 3 < 1){
						objs.push_back(new BreakWall(tmp, *this));
						display.addObj(objs[objs.size() - 1]->getId(),
							objs[objs.size() - 1]->getType(), objs[objs.size() - 1]->getPos());
					}
				}
			}
		}
	}
}

void Board::updateObjs()
{
	for (int j = 0; j < objs.size(); j++)
		objs[j]->update();
}

void Board::eraseObject(int id)
{
	for (int j = 0; j < objs.size(); j++)
		if (objs[j]->getId() == id){
			objs.erase(objs.begin() + j);
			i--;
			size--;
			break;
		}
	display.eraseObj(id);
}

bool Board::checkEmpty(int x, int y)
{
	for (i = 0; i < objs.size(); i++)
		if (objs[i]->getPos().x == x && objs[i]->getPos().y == y && objs[i]->isSolid())
			return false;
	return true;
}

bool Board::burn(Pos2D pos)
{
	size = objs.size();
	for (int i = 0; i < size; i++)
		if (objs[i]->getPos().x == pos.x && objs[i]->getPos().y == pos.y){
			if (objs[i]->getType() == FIRE_GAME){
				return 0;
			}
			if (objs[i]->isKillable()){
				objs[i]->onBomb();
			}
		}
	return 1;
}

int Board::checkFlameable(int x, int y)
{
	int num = 1;
	for (int i = 0; i < objs.size(); i++)
		if (objs[i]->getPos().x == x && objs[i]->getPos().y == y && objs[i]->isSolid()){
			if (objs[i]->getPos().x == x && objs[i]->getPos().y == y && objs[i]->isKillable()){
				num = 2;
			}
			else
				return 0;
		}
	return num;
}

void Board::moveDisplay(int id, char dir, float x, float y)
{
	display.moveObj(id, dir, x, y);
}

void	Board::playerMoved(int x, int y, Player& pl){
	size = objs.size();
	for (i = 0; i < size; i++){
		if (objs[i]->getPos().x == x && objs[i]->getPos().y == y){
			objs[i]->onPlayer(pl);
		}
	}
}

void	Board::enemyMoved(int x, int y, Enemy& en){
	
	for (int j = 0; j < objs.size(); j++){
		if (objs[j]->getPos().x == x && objs[j]->getPos().y == y){
			if (objs[j]->getType() == PLAYER_GAME){
				objs[j]->onBomb();
			}
			else if (objs[j]->getType() == FIRE_GAME){
				en.onBomb();
			}
		}
	}
}



Board::Board(int sX, int sY, Display& d) : display(d)
{
	bounds.x = sX;
	bounds.y = sY;
	initBoard();
}

