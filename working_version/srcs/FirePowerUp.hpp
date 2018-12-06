#ifndef FIREPUP_HPP
#define FIREPUP_HPP

#include "BombPowerUp.hpp"

class FirePowerUp : public BombPowerUp{
	public:
		FirePowerUp(Pos2D pos, Board& b);
		void onPlayer(Player&);
	private:

};

#endif