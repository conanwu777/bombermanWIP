#include "Display.hpp"

Display::Display(int wid, int hei) : tmpinput(0), width(wid), height(hei) {
	initscr();
	curs_set(0);
	halfdelay(1);
	w = newwin(height, width, 10, 10);
	refresh();
	// box(w, 0, 0);
	wrefresh(w);
	keypad(this->w, true);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_color(6, 824, 999, 266);
	init_pair(2, 6, COLOR_BLACK);
	wattron(w, COLOR_PAIR(1) | A_BOLD);
}

Display::~Display() {
	endwin();
}

Display::Display(Display const &rhs){
	*this = rhs;}

Display	&Display::operator=(Display const &rhs)
{
	if (this != &rhs)
		this->w = rhs.w;
	return (*this);
}

int Display::getInput() {
	tmpinput = wgetch(w);
	return tmpinput;
}

void	Display::addObj(int id, char type, Pos2D p)
{
	ViewObj *obj = new ViewObj(type, p);
	objs[id] = obj;
	mvwaddch(w, p.y, p.x, type);
	wrefresh(w);
}

void	Display::moveObj(int id, char direction, float amount)
{
	ViewObj *o = objs[id];
	Pos2D tmp = o->p;
	switch (direction) {
		case 'u':
			o->p.y -= amount;
			break ;
		case 'd':
			o->p.y += amount;
			break ;
		case 'l':
			o->p.x -= amount;
			break ;
		case 'r':
			o->p.x += amount;
			break ;
		default:
			return ;
	}
	mvwaddch(w, tmp.y, tmp.x, ' ');
	mvwaddch(w, o->p.y, o->p.x, o->type);
	wrefresh(w);
}
