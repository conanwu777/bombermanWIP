//
// Created by hjeannin on 28/07/17.
//

#ifndef TEXTURE_HPP
# define TEXTURE_HPP
# if defined(__APPLE_CC__)
#  ifndef GLFW_INCLUDE_GLCOREARB
#   define GLFW_INCLUDE_GLCOREARB
#  endif
#  ifndef GLFW_INCLUDE_GLEXT
#   define GLFW_INCLUDE_GLEXT
#  endif
# else
#  define GL_GLEXT_PROTOTYPES
# endif

# ifdef _WIN32
#  include <GL/glew.h>
# else
#  include <OpenGL/OpenGL.h>
# endif

# include <GLFW/glfw3.h>
# include <GLM/glm.hpp>
# include <iostream>
# define STB_IMAGE_IMPLEMENTATION
# define STB_IMAGE_STATIC
# include "stb_image.h"
# include <vector>

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    void    generateDefaultTexture(GLuint textureID);
    void    loadImageFile(GLuint textureID, const char * file, int wanted_bpp = 3);
    void    loadCubemap(GLuint textureID, std::vector<const GLchar*> faces, int wanted_bpp = 3);
    void    loadAtlas(GLuint textureID, char const *filename, int count, int wanted_bpp = 4);
    void    parametrizeFontTexture(GLuint textureID);
};

#endif // TEXTURE_HPP