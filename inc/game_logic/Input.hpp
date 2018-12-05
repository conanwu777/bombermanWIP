#ifndef INPUT_HPP
# define INPUT_HPP

# include <GLFW/glfw3.h>
# include "../../inc/game_logic/Board.hpp"

class Input {
	public :
		static void keyDown(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void setBoard(Board* b);
	private:
		static Board* board;
};

#endif
