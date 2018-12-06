#include "../../inc/game_logic/Display.hpp"

#include <vector>
#include <math.h>
#include <chrono>


Display::Display(int wid, int hei) : win("BomberBoy", W ,H), cam(), sh("src/game_logic/shaderpix/shaders/vertShader.vs", "src/game_logic/shaderpix/shaders/regFrag.fs"), in(cam, win), tmpinput(0), width(wid * 10), height(hei * 5) {
	

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(win.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// sh = Shader("shaders/vertShader.vs", "shaders/regFrag.fs");
	// Shader uiShader("shaders/ui.vs", "shaders/regFrag.fs");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);
}

void	Display::renderLoop(){
	glClearColor(0.0859f, 0.121f, 0.226f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (map<int, ViewObj*>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		it->second->obj->SetCurrentRender();
		it->second->obj->Render();
	}
	
	in.KeyHold(win);

	win.Update();
}

Display::~Display() {
	// endwin();
}

Display::Display(Display const &rhs) :  win("BomberBoy", W ,H), cam(), sh("src/game_logic/shaderpix/shaders/vertShader.vs", "src/game_logic/shaderpix/shaders/regFrag.fs"), in(cam, win)
{
	*this = rhs;}

Display	&Display::operator=(Display const &rhs) 
{
	// if (this != &rhs)
		// this->w = rhs.w;
	// return (*this);
}

int Display::getInput() {
	// tmpinput = wgetch(w);
	return 'c'; //FIX ME BOY
}

void	Display::addObj(int id, objectType type, Pos2D p)
{
	Cube* cube;
	switch (type){
		case PLAYER_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, "src/game_logic/shaderpix/texture/player.png");
			break;
		case WALL_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, "src/game_logic/shaderpix/texture/wall.png");
			break;

		case BREAKABLE_WALL_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, "src/game_logic/shaderpix/texture/break.png");
			break;

		case BOMB_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, "src/game_logic/shaderpix/texture/bomb.png");
			break;

		case FIRE_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, "src/game_logic/shaderpix/texture/fire.png");
			break;

		case BOMB_POWER_UP_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, "src/game_logic/shaderpix/texture/firePup.png");
			break;

		case FIRE_POWER_UP_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, "src/game_logic/shaderpix/texture/firePup.png");
			break;
	
	}
	cube->Activate();


	ViewObj *obj = new ViewObj(type, p);
	obj->obj = cube;
	objs[id] = obj;
}

void	Display::moveObj(int id, char dir, float x, float y)
{
	ViewObj *o = objs[id];

	o->obj->pos[0] = x;
	o->obj->pos[2] = y;
}

void	Display::eraseObj(int id)
{
	ViewObj *o = objs[id];
	objs.erase(id);
	// mvwaddch(w, o->p.y, o->p.x, ' ');
	free(o);
}