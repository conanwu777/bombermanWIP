#include "SkatesPowerUp.hpp"

SkatesPowerUp::SkatesPowerUp(Pos2D pos, Board& b) : BombPowerUp(pos, b, SKATE_POWER_UP_GAME) {

}

void	SkatesPowerUp::onPlayer(Player& player){
	player.speedUp();
	board.eraseObject(id);
	delete this;
}