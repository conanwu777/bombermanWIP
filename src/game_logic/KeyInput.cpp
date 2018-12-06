#include "../../inc/game_logic/KeyInput.hpp"
#include "../../inc/game_logic/Object.hpp"
#include "../../inc/game_logic/Player.hpp"
#include <iostream>

Board* KeyInput::board;

void KeyInput::keyDown(GLFWwindow* window, int key, int scancode, int action, int mods){
	
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		dynamic_cast<Player*>(board->objs[0])->dropBomb();
}

void KeyInput::checkInput(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_UP)){
		board->objs[0]->move('u');
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN)){
		board->objs[0]->move('d');
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT)){
		board->objs[0]->move('l');
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT)){
		board->objs[0]->move('r');
	}


}

void KeyInput::setBoard(Board* b){
	board = b;
}
