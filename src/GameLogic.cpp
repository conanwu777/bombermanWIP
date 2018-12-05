//
// Created by Hugo JEANNIN on 12/5/18.
//

#include "../inc/GameLogic.hpp"

GameLogic::GameLogic()
{
    std::cerr << "GameLogic() constructor called." << std::endl;
}

GameLogic::~GameLogic()
{
    std::cerr << "GameLogic() destructor called." << std::endl;
}

void GameLogic::init()
{
    // Here I need only the game logic nothing about view/rendering
    // I ll take care of everything visual.
}