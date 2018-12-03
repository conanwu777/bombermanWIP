#include "Display.hpp"

ViewObj::ViewObj() : type(0) {
	p.x = 0;
	p.y = 0;
}

ViewObj::ViewObj(char type, Pos2D p) : type(type), p(p) {}

ViewObj::~ViewObj() {}

ViewObj::ViewObj (ViewObj const &rhs) {
	*this = rhs;
}

ViewObj & ViewObj::operator=(ViewObj const &rhs) {
	type = rhs.type;
	p = rhs.p;
	return (*this);
}
