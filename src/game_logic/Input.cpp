#include "../../inc/game_logic/Input.hpp"
#include "../../inc/game_logic/Object.hpp"
#include "../../inc/game_logic/Player.hpp"
#include <iostream>

Board* Input::board;

void Input::keyDown(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		board->objs[0]->move('u');
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		board->objs[0]->move('d');
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		board->objs[0]->move('l');
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		board->objs[0]->move('r');
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		dynamic_cast<Player*>(board->objs[0])->dropBomb();
}

void Input::setBoard(Board* b){
	board = b;
}
