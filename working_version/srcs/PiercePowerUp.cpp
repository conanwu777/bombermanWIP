#include "PiercePowerUp.hpp"

PiercePowerUp::PiercePowerUp(Pos2D pos, Board& b) : BombPowerUp(pos, b, PIERCE_POWER_UP_GAME) {

}

void	PiercePowerUp::onPlayer(Player& player){
	player.activatePierce();
	board.eraseObject(id);
	delete this;
}