#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Position.hpp"
#include "Board.hpp"
#include "../Constant.hpp"

class Player;


class Object{
	public:
		Object(objectType type, int life, bool killable, bool solid, Pos2D pos, Board& b);
		Pos2D getPos();
		int getId();
		objectType getType();
		bool isSolid();
		bool isMoving();
		bool isKillable();
		virtual void move(char dir){};
		virtual void update(){};
		virtual void onBomb(){};
		virtual void onPlayer(Player& player){};
		int life;
		bool alive;

	protected:
		static int numIds;
		Pos2D pos;
		PosF2D off;
		int id;
		objectType type;
		bool moving;
		bool solid;
		bool killable;
		Board& board;
};

#endif