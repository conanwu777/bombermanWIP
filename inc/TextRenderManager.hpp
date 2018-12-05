//
// Created by Remi DAVID on 10/18/18.
//

#ifndef TEXT_RENDER_MANAGER_HPP
#define TEXT_RENDER_MANAGER_HPP

# if defined(_WIN32)
#  include <GL/glew.h>
# else
#  include <OpenGL/OpenGL.h>
# endif

# include "GLFW/glfw3.h"

# include <iostream>
# include <cmath>

class TextRenderManager
{
public:
    TextRenderManager();
    ~TextRenderManager();

    void init();

private:
};

#endif //TEXT_RENDER_MANAGER_HPP
