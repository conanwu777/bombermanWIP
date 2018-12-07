#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "general.hpp"
# include "Position.hpp"
# include "ViewObj.hpp"
# include "Constant.hpp"

#include "../engine/srcs/graphics.hpp"
#include "../engine/srcs/Shader.hpp"
#include "../engine/srcs/RenderObject.hpp"
#include "../engine/srcs/Cube.hpp"
#include "../engine/srcs/Panel.hpp"
#include "../engine/srcs/Window.hpp"
#include "../engine/srcs/Camera.hpp"
#include "../engine/srcs/Input.hpp"

class Display {
		map<int, ViewObj*> objs;
		int width, height;

	public:
		int tmpinput;

		unsigned int textures[20];

		Window win;
		Camera cam;
		Shader sh;
		Shader uiSh;
		Input in;
		Display(int width, int height);
		~Display();
		Display (Display const &rhs);
		Display & operator=(Display const &rhs);
		void	renderLoop();
		int	getInput();
		void	loadTexture(char *str, int num);
		void	addObj(int id, objectType type, Pos2D p);
		void	moveObj(int id, char dir, float x, float y);
		void	eraseObj(int id);
};

#endif
