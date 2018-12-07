#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "Position.hpp"
#include "Display.hpp"

class Object;
class Player;
class Enemy;

using namespace std;

class Board{
	public:
		vector<Object*> objs;
		Pos2D bounds;
		Board(int sX, int sY, Display& d);
		void swap(int x, int y, int nX, int nY);
		void moveDisplay(int, char, float, float);
		void initBoard();
		void updateObjs();
		void enemyMoved(int x, int y, Enemy& en);
		void playerMoved(int x, int y, Player& pl);
		bool checkEmpty(int x, int y);
		int checkFlameable(int x, int y);
		bool burn(Pos2D pos);
		void eraseObject(int id);
		Display& display;

	private:
		int i;
		int size;

};

#endif