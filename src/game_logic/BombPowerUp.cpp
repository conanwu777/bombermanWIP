#include "../../inc/game_logic/BombPowerUp.hpp"

BombPowerUp::BombPowerUp(Pos2D pos, Board& b) : Object('b', 0, true, false, pos, b){

}

BombPowerUp::BombPowerUp(Pos2D pos, Board& b, char c) : Object(c, 0, true, false, pos, b){

}

void	BombPowerUp::onBomb(){
	board.eraseObject(id);
	delete this;
}

void	BombPowerUp::onPlayer(Player& player){
	player.addBomb();
	board.eraseObject(id);
	delete this;
}