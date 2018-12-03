#include "general.hpp"
#include "Display.hpp"

int main()
{
	Display d;

	pos p = {15, 15};
	d.addObj(1, '#', p);
	p.x += 10;
	d.addObj(2, '*', p);
	p.x += 10;
	d.addObj(3, '*', p);


	while (d.tmpinput != 'q')
	{
		d.getInput();
		d.moveObj(1, d.tmpinput, 1);
	}

	return 0;
}