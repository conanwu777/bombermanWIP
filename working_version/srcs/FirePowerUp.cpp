#include "FirePowerUp.hpp"

FirePowerUp::FirePowerUp(Pos2D pos, Board& b) : BombPowerUp(pos, b, FIRE_POWER_UP_GAME) {

}

void	FirePowerUp::onPlayer(Player& player){
	player.incRange(1);
	board.eraseObject(id);
	delete this;
}