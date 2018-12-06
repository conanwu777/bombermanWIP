//
// Created by Hugo JEANNIN on 12/5/18.
//

#ifndef GAMELOGIC_HPP
# define GAMELOGIC_HPP

# include <iostream>
# include <GLM/glm.hpp>
# include "../inc/game_logic/Board.hpp"
# include "../inc/game_logic/Object.hpp"
# include "../inc/game_logic/Player.hpp"


class GameLogic
{
public:

    struct GameObject
    {
        char            type;
        glm::mat4       matrix;
        GLuint          texture_id;
    };

    std::vector<GameObject>     game_objects;

    GameLogic();

    ~GameLogic();

    void init();
};


#endif //GAMELOGIC_HPP
