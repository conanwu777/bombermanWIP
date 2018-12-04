#include "Board.hpp"
#include "Object.hpp"
#include "Time.hpp"
//forncurse
#include "general.hpp"

int main()
{
	Time::updateTime();

	Display d(11, 11);
	Board b(11, 11, d);

	while(1){
		Time::updateTime();
		int in = d.getInput();
		if (in == KEY_UP){
			b.board[b.pX][b.pY]->move('u');
		}
		if (in == KEY_DOWN){
			b.board[b.pX][b.pY]->move('d');
		}
		if (in == KEY_LEFT){
			b.board[b.pX][b.pY]->move('l');
		}
		if (in == KEY_RIGHT){
			b.board[b.pX][b.pY]->move('r');
		}
	}
}