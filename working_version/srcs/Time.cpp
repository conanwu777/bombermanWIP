#include "Time.hpp"
#include "general.hpp"

float Time::curTime;

void Time::updateTime(){
	curTime = glfwGetTime();
}