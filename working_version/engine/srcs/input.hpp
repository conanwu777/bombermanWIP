#ifndef INPUT_HPP
#define INPUT_HPP

#include "Camera.hpp"
#include "Window.hpp"
#include "graphics.hpp"

class PostProcess;

class Input{
	public:
		float curTime;
		float startTime;
		float lastTime;

		Input(Camera& c, Window& w);
		Camera& cam;
		Window& win;
		void KeyHold(Window& win);
		// void transparentCheck(Position&, vector<RenderObject*> &panels);
		// void frameCheck(Position&, Position& movCam, vector<RenderObject*> &panels);
		float   dist(Position& a, Position& b);
		bool spaceDown = false;
		bool oneDown = false;
		int obj;
		static Input* singleton;
};

#endif