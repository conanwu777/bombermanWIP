//
// Created by Hugo JEANNIN on 3/10/17.
//

#ifndef CORE_HPP

# define _USE_MATH_DEFINES
# include <cmath>

# include "Skybox.hpp"
# include "Axes.hpp"
# include "RenderManager.hpp"
# include "CLWrapper.hpp"
# include "TextObject.hpp"
# include "OBJObject.hpp"
# include "AnimationManager.hpp"
# include "Cube.hpp"
# include "GameLogic.hpp"

class Core
{
public:
    CLWrapper      cl_wrapper;

    Skybox        skybox;

    Axes          axes;

    TextObject    fps_counter;

    GameLogic     game_logic;

    OBJObject     tree; /// Must give each object an ID
    OBJObject     tree_2; /// Must give each object an ID

    Cube          cube;

    RenderManager  render_manager;

    AnimationManager animation_manager;

    Core();

    ~Core();

    void init();

    void update(RenderManager *render_manager);

    void handleKeyEvent(int key, int scancode, int action, int mods);
    void handleCursorEvent(double xpos, double ypos);
    void handleMouseButtonCallback(int button, int action, int mods);

private:
    bool _show_debug_info;

};

#endif //CORE_HPP
