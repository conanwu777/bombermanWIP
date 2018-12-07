#include "Board.hpp"
#include "Object.hpp"
#include "Player.hpp"
#include "Time.hpp"
#include "KeyInput.hpp"
//forncurse
#include "general.hpp"
#include <cstdlib>

int main()
{
	glfwInit();

	Time::updateTime();

	srand(time(NULL));

	Display d(11, 11);
	Board b(11, 11, d);

	//DOES nOTHING FOR NOW, pass d.window once that's a thing
	KeyInput::setBoard(&b);
	glfwSetKeyCallback(d.win.window, KeyInput::keyDown);

	while(!glfwWindowShouldClose(d.win.window)){
		Time::updateTime();
		b.updateObjs();
		KeyInput::checkInput(d.win.window);

		// int in = d.getInput();
		// if (in == KEY_UP){
		// 	b.objs[0]->move('u');
		// }
		// if (in == KEY_DOWN){
		// 	b.objs[0]->move('d');
		// }
		// if (in == KEY_LEFT){
		// 	b.objs[0]->move('l');
		// }
		// if (in == KEY_RIGHT){
		// 	b.objs[0]->move('r');
		// }
		// if (in == ' '){
		// 	// std::string command = "afplay test.wav";
  //   		// system(command.c_str());
		// 	dynamic_cast<Player*>(b.objs[0])->dropBomb();
		// }
		d.renderLoop();
	}
}