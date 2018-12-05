#include "../../inc/game_logic/Board.hpp"
#include "../../inc/game_logic/Wall.hpp"
#include "../../inc/game_logic/BreakWall.hpp"
#include "../../inc/game_logic/Player.hpp"
#include "../../inc/game_logic/BombPowerUp.hpp"
#include "../../inc/game_logic/FirePowerUp.hpp"

void Board::initBoard(){

	Pos2D tmp;

	objs.reserve(bounds.x * bounds.y * sizeof(Object*));
	
	tmp.x = 1;
	tmp.y = 1;
	objs.push_back(new Player(tmp, *this));
	display.addObj(objs[objs.size() - 1]->getId(),
		objs[objs.size() - 1]->getType(), objs[objs.size() - 1]->getPos());
	tmp.x = 3;

	objs.push_back(new BombPowerUp(tmp, *this));
	display.addObj(objs[objs.size() - 1]->getId(),
		objs[objs.size() - 1]->getType(), objs[objs.size() - 1]->getPos());
	tmp.x = 2;

	objs.push_back(new FirePowerUp(tmp, *this));
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
			else if(x + y > 6 && x + y < bounds.x + bounds.y - 6){
				objs.push_back(new BreakWall(tmp, *this));
				display.addObj(objs[objs.size() - 1]->getId(),
					objs[objs.size() - 1]->getType(), objs[objs.size() - 1]->getPos());
			}
		}
	}
}

void Board::updateObjs()
{
	for (i = 0; i < objs.size(); i++)
		objs[i]->update();
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

void Board::burn(Pos2D pos)
{
	size = objs.size();
	for (int i = 0; i < size; i++)
		if (objs[i]->getPos().x == pos.x && objs[i]->getPos().y == pos.y && objs[i]->isKillable()){
			objs[i]->onBomb();
		}
}

int Board::checkFlameable(int x, int y)
{
	int num = 1;
	for (int i = 0; i < objs.size(); i++)
		if (objs[i]->getPos().x == x && objs[i]->getPos().y == y && objs[i]->isSolid()){
			if (objs[i]->getPos().x == x && objs[i]->getPos().y == y && objs[i]->isKillable())
				num = 2;
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


Board::Board(int sX, int sY, Display& d) : display(d)
{
	bounds.x = sX;
	bounds.y = sY;
	initBoard();
}

