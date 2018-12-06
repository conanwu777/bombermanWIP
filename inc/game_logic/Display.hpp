#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "general.hpp"
# include "Position.hpp"
# include "ViewObj.hpp"
#include "../Constant.hpp"

#include "../../src/game_logic/shaderpix/srcs/graphics.hpp"
#include "../../src/game_logic/shaderpix/srcs/Shader.hpp"
#include "../../src/game_logic/shaderpix/srcs/RenderObject.hpp"
#include "../../src/game_logic/shaderpix/srcs/Cube.hpp"
#include "../../src/game_logic/shaderpix/srcs/Window.hpp"
#include "../../src/game_logic/shaderpix/srcs/Camera.hpp"
#include "../../src/game_logic/shaderpix/srcs/Input.hpp"

class Display {
		map<int, ViewObj*> objs;
		int width, height;

	public:
		int tmpinput;

		Window win;
		Camera cam;
		Shader sh;
		Input in;
		Display(int width, int height);
		~Display();
		Display (Display const &rhs);
		Display & operator=(Display const &rhs);
		void	renderLoop();
		int	getInput();
		void	addObj(int id, objectType type, Pos2D p);
		void	moveObj(int id, char dir, float x, float y);
		void	eraseObj(int id);
};

#endif
