#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "Position.hpp"
#include "Display.hpp"

class Object;

using namespace std;

class Board{
	public:
		vector<Object*> objs;
		Pos2D bounds;
		Board(int sX, int sY, Display& d);
		void swap(int x, int y, int nX, int nY);
		void moveDisplay(int, char, float);
		void initBoard();
		void updateObjs();
		bool checkWalk(int x, int y);
		void eraseObject(int id);
		Display& display;

};

#endif