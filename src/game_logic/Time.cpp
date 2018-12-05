#include "../../inc/game_logic/Time.hpp"
#include "../../inc/game_logic/general.hpp"

float Time::curTime;

void Time::updateTime(){
	curTime = glfwGetTime();
}