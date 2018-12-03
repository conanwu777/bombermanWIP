#ifndef VIEWOBJ_HPP
# define VIEWOBJ_HPP

class ViewObj {
	public:
		char type;
		pos	p;
		ViewObj();
		ViewObj(char type, pos p);
		~ViewObj();
		ViewObj (ViewObj const &rhs);
		ViewObj & operator=(ViewObj const &rhs);
};

#endif