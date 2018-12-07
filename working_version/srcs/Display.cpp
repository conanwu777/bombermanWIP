#include "Display.hpp"
#include "../engine/texture/stb_image.h"

#include <vector>
#include <math.h>
#include <chrono>

Display::Display(int wid, int hei) : win("BomberBoy", W ,H), cam(),
sh("engine/shaders/vertShader.vs", "engine/shaders/regFrag.fs"),
uiSh("engine/shaders/ui.vs", "engine/shaders/regFrag.fs"),
in(cam, win), tmpinput(0), width(wid), height(hei) {
	
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(win.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// sh = Shader("shaders/vertShader.vs", "shaders/regFrag.fs");
	// Shader uiShader("shaders/ui.vs", "shaders/regFrag.fs");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);

	loadTexture("engine/texture/player.png", PLAYER_GAME);
	loadTexture("engine/texture/wall.png", WALL_GAME);
	loadTexture("engine/texture/break.png", BREAKABLE_WALL_GAME);
	loadTexture("engine/texture/bomb.png", BOMB_GAME);
	loadTexture("engine/texture/fire.png", FIRE_GAME);
	loadTexture("engine/texture/firePup.png", FIRE_POWER_UP_GAME);
	loadTexture("engine/texture/firePup.png", BOMB_POWER_UP_GAME);
	loadTexture("engine/texture/bomb.png", ENEMY_GAME);

	// Panel* pan = new Panel(uiSh, cam, 0, 0, 0, 1, 1, 1, textures[PLAYER_GAME]);
	
	// pan->Activate();

	// Pos2D tmp;
	// tmp.x = 0;
	// tmp.y = 0;
	// ViewObj *obj = new ViewObj(PLAYER_GAME, tmp);
	// obj->obj = pan;
	// objs[1000] = obj;
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

Display::Display(Display const &rhs) :  win("BomberBoy", W ,H), cam(),
sh("engine/shaders/vertShader.vs", "engine/shaders/regFrag.fs"), uiSh("engine/shaders/ui.vs", "engine/shaders/regFrag.fs"), in(cam, win)
{
	*this = rhs;
}

void	Display::loadTexture(char *tex, int pos){
	int width, height, nrChannels;
	unsigned int texture;
	unsigned char *data = stbi_load(tex, &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	textures[pos] = texture;
	stbi_image_free(data);
	
}

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
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, textures[PLAYER_GAME]);
			break;
		case WALL_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, textures[WALL_GAME]);
			break;

		case BREAKABLE_WALL_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, textures[BREAKABLE_WALL_GAME]);
			break;

		case BOMB_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, textures[BOMB_GAME]);
			break;

		case FIRE_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, textures[FIRE_GAME]);
			break;

		case BOMB_POWER_UP_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, textures[BOMB_POWER_UP_GAME]);
			break;

		case FIRE_POWER_UP_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, textures[FIRE_POWER_UP_GAME]);
			break;

		case ENEMY_GAME:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, textures[ENEMY_GAME]);
			break;

		default:
			cube = new Cube(sh, cam, p.x, -7, p.y, 1, 1, 1, textures[PLAYER_GAME]);
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