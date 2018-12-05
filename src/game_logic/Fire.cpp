#include "../../inc/game_logic/Fire.hpp"
#include "../../inc/game_logic/Time.hpp"

Fire::Fire(Pos2D p, Board& b, int range, char dir) : Object(FIRE_GAME, 0, false, false, p, b){
	startTime = Time::curTime;
	duration = 1;

	board.burn(pos);

	if (range == 0){
		return;
	}
	int flameRange; 
	if ((dir == 'a' || dir == 'u') && (flameRange = board.checkFlameable(pos.x, pos.y - 1))){
		pos.y -= 1;
		board.objs.push_back(new Fire(pos, board, (flameRange == 1 ? range - 1 : 0), 'u'));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
			board.objs[board.objs.size() - 1]->getType(),
			board.objs[board.objs.size() - 1]->getPos());
		pos.y += 1;
	}
	if ((dir == 'a' || dir == 'd') && (flameRange = board.checkFlameable(pos.x, pos.y + 1))){
		pos.y += 1;
		board.objs.push_back(new Fire(pos, board, (flameRange == 1 ? range - 1 : 0), 'd'));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
			board.objs[board.objs.size() - 1]->getType(),
			board.objs[board.objs.size() - 1]->getPos());
		pos.y -= 1;
	}
	if ((dir == 'a' || dir == 'l') && (flameRange = board.checkFlameable(pos.x - 1, pos.y))){
		pos.x -= 1;
		board.objs.push_back(new Fire(pos, board, (flameRange == 1 ? range - 1 : 0), 'l'));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
			board.objs[board.objs.size() - 1]->getType(),
			board.objs[board.objs.size() - 1]->getPos());
		pos.x += 1;
	}
	if ((dir == 'a' || dir == 'r') && (flameRange = board.checkFlameable(pos.x + 1, pos.y))){
		pos.x += 1;
		board.objs.push_back(new Fire(pos, board, (flameRange == 1 ? range - 1 : 0), 'r'));
		board.display.addObj(board.objs[board.objs.size() - 1]->getId(),
			board.objs[board.objs.size() - 1]->getType(),
			board.objs[board.objs.size() - 1]->getPos());
		pos.x -= 1;
	}
}

void	Fire::update()
{
	if (Time::curTime > startTime + duration){
		board.eraseObject(id);
		delete this;
	}
}