#ifndef SKATESPUP_HPP
#define SKATESPUP_HPP

#include "BombPowerUp.hpp"

class SkatesPowerUp : public BombPowerUp {
	public:
		SkatesPowerUp(Pos2D pos, Board& b);
		void onPlayer(Player&);
	private:

};

#endif