#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "general.hpp"
# include "ViewObj.hpp"

class Display {
		WINDOW *w;
		map<int, ViewObj*> objs;
		static const int ym = H;
		static const int xm = W;

	public:
		char tmpinput;

		Display();
		~Display();
		Display (Display const &rhs);
		Display & operator=(Display const &rhs);
		void	getInput();
		void	addObj(int id, char type, pos p);
		void	moveObj(int id, char direction, float amount);
};

#endif
