#ifndef PIERCEPUP_HPP
#define PIERCEPUP_HPP

#include "BombPowerUp.hpp"

class PiercePowerUp : public BombPowerUp{
	public:
		PiercePowerUp(Pos2D pos, Board& b);
		void onPlayer(Player&);
	private:

};

#endif