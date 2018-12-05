#include "../../inc/game_logic/Board.hpp"
#include "../../inc/game_logic/Object.hpp"
#include "../../inc/game_logic/Player.hpp"
#include "../../inc/game_logic/Time.hpp"
//forncurse
#include "../../inc/game_logic/general.hpp"

int main()
{
	glfwInit();

	Time::updateTime();

	Display d(11, 11);
	Board b(11, 11, d);

	while(1){
		Time::updateTime();
		b.updateObjs();
		int in = d.getInput();
		if (in == KEY_UP){
			b.objs[0]->move('u');
		}
		if (in == KEY_DOWN){
			b.objs[0]->move('d');
		}
		if (in == KEY_LEFT){
			b.objs[0]->move('l');
		}
		if (in == KEY_RIGHT){
			b.objs[0]->move('r');
		}
		if (in == ' '){
			dynamic_cast<Player*>(b.objs[0])->dropBomb();
		}
	}
}