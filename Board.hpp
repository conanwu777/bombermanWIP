#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "Position.hpp"
#include "Display.hpp"

class Object;

using namespace std;

class Board{
	public:
		vector<vector<Object*> > board;
		Pos2D bounds;
		Board(int sX, int sY, Display& d);
		void swap(int x, int y, int nX, int nY);
		void moveDisplay(int, char, float);
		void initBoard();
		bool checkWalk(int x, int y);
		int pX;
		int pY;

	private:
		Display& display;

};

#endif