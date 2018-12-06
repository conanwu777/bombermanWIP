//
// Created by Hugo JEANNIN on 12/5/18.
//

#include <GLM/ext.hpp>
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
    game_objects.push_back({PLAYER_GAME, glm::mat4(1.0f), DEFAULT_GENERATED_TEXTURE});
    game_objects.push_back({BOMB_GAME, glm::mat4(1.0f), CHECKER_TEXTURE});
    game_objects[1].matrix *= glm::translate(game_objects[1].matrix, glm::vec3(3.0f, 0.0f, 0.0f));
}