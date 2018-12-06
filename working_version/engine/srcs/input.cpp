#include "input.hpp"
#include "math.h"
#include <vector>

Input* Input::singleton;

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if (Input::singleton->cam.first){
		Input::singleton->cam.xMouse = xpos;
		Input::singleton->cam.yMouse = ypos;
		Input::singleton->cam.first = false;
	}

	Input::singleton->cam.lookHoriz(xpos - Input::singleton->cam.xMouse);
	Input::singleton->cam.lookVert(ypos - Input::singleton->cam.yMouse);
	Input::singleton->cam.xMouse = xpos;
	Input::singleton->cam.yMouse = ypos;
}

Input::Input(Camera& c, Window& w)
: cam(c), win(w)
{
	Input::singleton = this;
	glfwSetCursorPosCallback(win.window, &mouse_callback);
	startTime = glfwGetTime();
}

float   Input::dist(Position& a, Position& b){
	return (sqrt(pow(a.x + b.x, 2) + pow(a.y + b.y, 2)));
}

void Input::KeyHold(Window& win){
	curTime = glfwGetTime();
	float deltaTime = curTime - lastTime;
	lastTime = glfwGetTime();

	if(glfwGetKey(win.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win.window, true);
	Position newCam = {cam.pos[0],cam.pos[2]};
	Position movCam = {0,0};

	float	moveSpeed = deltaTime * 15.0f;

	if(glfwGetKey(win.window, GLFW_KEY_W) == GLFW_PRESS){
		newCam.y += cos(cam.getRotation()) * moveSpeed;
		movCam.y += cos(cam.getRotation()) * 10 * moveSpeed;
		newCam.x -= sin(cam.getRotation()) * moveSpeed;
		movCam.x += -sin(cam.getRotation()) * 10 * moveSpeed;
	}
	if(glfwGetKey(win.window, GLFW_KEY_S) == GLFW_PRESS){
		newCam.y -= cos(cam.getRotation()) * moveSpeed;
		movCam.y += -cos(cam.getRotation()) * 10 * moveSpeed;
		newCam.x += sin(cam.getRotation()) * moveSpeed;
		movCam.x += sin(cam.getRotation()) * 10 * moveSpeed;
	}
	if(glfwGetKey(win.window, GLFW_KEY_A) == GLFW_PRESS){
		newCam.x += cos(cam.getRotation()) * moveSpeed;
		movCam.x += cos(cam.getRotation()) * 10 * moveSpeed;
		newCam.y += sin(cam.getRotation()) * moveSpeed;
		movCam.y += sin(cam.getRotation()) * 10 * moveSpeed;
	}
	if(glfwGetKey(win.window, GLFW_KEY_D) == GLFW_PRESS){
		newCam.x -= cos(cam.getRotation()) * moveSpeed;
		movCam.x += -cos(cam.getRotation()) * 10 * moveSpeed;
		newCam.y -= sin(cam.getRotation()) * moveSpeed;
		movCam.y += -sin(cam.getRotation()) * 10 * moveSpeed;
	}
	if(glfwGetKey(win.window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		spaceDown = false;
	if(glfwGetKey(win.window, GLFW_KEY_1) == GLFW_RELEASE)
		oneDown = false;
	if (newCam.x != cam.pos[0] || newCam.y != cam.pos[2]){
		cam.pos[0] = newCam.x;
		cam.pos[2] = newCam.y;
		cam.reload();
	}
	
	if (glfwGetKey(win.window, GLFW_KEY_SPACE) == GLFW_PRESS)
		spaceDown = true;
	
	
}