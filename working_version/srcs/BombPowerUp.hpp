#ifndef BOMBPUP_HPP
#define BOMBPUP_HPP

#include "Position.hpp"
#include "Object.hpp"
#include "Player.hpp"

class BombPowerUp : public Object{
	public:
		BombPowerUp(Pos2D pos, Board& b);
		BombPowerUp(Pos2D pos, Board& b, objectType c);
		void onBomb();
		virtual void onPlayer(Player&);
	private:

};

#endif