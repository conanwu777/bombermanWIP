#ifndef VIEWOBJ_HPP
# define VIEWOBJ_HPP

#include "Position.hpp"

class ViewObj {
	public:
		char type;
		Pos2D	p;
		ViewObj();
		ViewObj(char type, Pos2D p);
		~ViewObj();
		ViewObj (ViewObj const &rhs);
		ViewObj & operator=(ViewObj const &rhs);
};

#endif