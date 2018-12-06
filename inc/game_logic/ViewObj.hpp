#ifndef VIEWOBJ_HPP
# define VIEWOBJ_HPP

#include "Position.hpp"
#include "../../src/game_logic/shaderpix/srcs/RenderObject.hpp"

class ViewObj {
	public:
		char type;
		Pos2D	p;
		RenderObject* obj;
		ViewObj();
		ViewObj(char type, Pos2D p);
		~ViewObj();
		ViewObj (ViewObj const &rhs);
		ViewObj & operator=(ViewObj const &rhs);
};

#endif