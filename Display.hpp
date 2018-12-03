#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "general.hpp"
# include "Position.hpp"
# include "ViewObj.hpp"

class Display {
		WINDOW *w;
		map<int, ViewObj*> objs;
		int width, height;

	public:
		int tmpinput;

		Display(int width, int height);
		~Display();
		Display (Display const &rhs);
		Display & operator=(Display const &rhs);
		int	getInput();
		void	addObj(int id, char type, Pos2D p);
		void	moveObj(int id, char direction, float amount);
};

#endif
