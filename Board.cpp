#include "Board.hpp"
#include "Wall.hpp"
#include "Player.hpp"

void Board::initBoard(){

	Pos2D tmp;

	board.reserve(bounds.x * sizeof(vector<Object*>));
	for (int x = 0; x < bounds.x; x++){
		tmp.x = x;
		board[x].reserve(bounds.y * sizeof(Object*));
		for (int y = 0; y < bounds.y; y++){
			tmp.y = y;
			board[x][y][0] = NULL;
			if (x == 0 || y == 0 || x == bounds.x - 1 || y == bounds.y - 1 || (x % 2 == 0 && y % 2 == 0)){
				board[x][y][0] = new Wall(tmp, *this);
				display.addObj(board[x][y]->getId(), board[x][y]->getType(), board[x][y]->getPos());
			}
		}
	}
	tmp.x = 7;
	tmp.y = 1;
	board[7][1][0] = new Player(tmp, *this);
	display.addObj(board[7][1][0]->getId(), 'p', board[7][1][0]->getPos());
	pX = 7;
	pY = 1;

}

bool Board::checkWalk(int x, int y){
	if (board[x][y][0] == NULL || !board[x][y][0]->isSolid()){
		return true;
	}
	return false;
}

void Board::swap(int x, int y, int nX, int nY){
	board[nX][nY][0] = board[x][y];
	board[x][y][0] = NULL;
}

void Board::moveDisplay(int id, char dir, float amount){
	display.moveObj(id, dir, amount);
}

Board::Board(int sX, int sY, Display& d) : display(d){
	bounds.x = sX;
	bounds.y = sY;

	initBoard();
}

