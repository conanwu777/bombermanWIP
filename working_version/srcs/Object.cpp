#include "Object.hpp"

int Object::numIds = 0;

Object::Object(objectType t, int l, bool k, bool s, Pos2D p, Board& b): board(b){
	type = t; 
	life = l;
	killable = k;
	solid = s;
	pos = p;
	alive = true;
	id = numIds;
	numIds++;
	moving = false;
	off.x = 0;
	off.y = 0;
}

Pos2D Object::getPos(){
	return pos;
}

int Object::getId(){
	return id;
}

objectType Object::getType(){
	return type;
}

bool Object::isSolid(){
	return solid;
}

bool Object::isKillable(){
	return killable;
}

bool Object::isMoving(){
	return moving;
}