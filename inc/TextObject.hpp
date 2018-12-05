//
// Created by Remi DAVID on 10/18/18.
//

#ifndef TEXT_OBJECT_HPP
#define TEXT_OBJECT_HPP

#include <map>
#include "RenderManager.hpp"
#include "IRenderedObject.hpp"

class TextObject : IRenderedObject
{
public:
    struct Character
    {
        GLuint      TextureID;  // ID handle of the glyph texture
        glm::ivec2  Size;       // Size of glyph
        glm::ivec2  Bearing;    // Offset from baseline to left/top of glyph
        GLuint      Advance;    // Offset to advance to next glyph
    };

    float           max_y = 0.0f;
    std::string     text;
    GLfloat         line_pos_x = 0.0f;
    GLfloat         line_pos_y = 0.0f;
    GLfloat         scale = 1.0f;

    std::map<GLchar, Character>  Characters;

    TextObject();

    ~TextObject();

    void loadTexture(RenderManager const &render_manager);

    void initGL(RenderManager const &render_manager);

    void preRender(RenderManager const &render_manager);

    void render(RenderManager const &render_manager);

private:
    GLuint          _vao;
    GLuint          _vbo;
    GLuint          _color_loc;
    glm::mat4       _orthographic_matrix;

};

#endif //TEXT_OBJECT_HPP
