//
// Created by Hugo JEANNIN on 10/18/18.
//

#include "../inc/TextObject.hpp"

TextObject::TextObject(void)
{

}

TextObject::~TextObject(void)
{

}

void TextObject::loadTexture(const RenderManager &render_manager)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "../assets/fonts/FreeSans.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 30);

    glActiveTexture(GL_TEXTURE3);
    /// Load every Textures
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (GLuint) face->glyph->advance.x
        };
        if (face->glyph->bitmap_top >= max_y)
            max_y = face->glyph->bitmap_top;
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    // clean
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

}

void TextObject::initGL(const RenderManager &render_manager)
{
    loadTexture(render_manager);

    _orthographic_matrix = glm::ortho(0.0f, static_cast<GLfloat>(INITIAL_RES_X), static_cast<GLfloat>(INITIAL_RES_Y), 0.0f);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(RenderManager::position_loc);
    glVertexAttribPointer(RenderManager::position_loc, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, nullptr);
    /// clean
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _program_id = render_manager.getShaderProgramID(TEXT_SHADER);
    _texture_id = FONT_TEXTURE;
    _mvp_matrix_loc = (GLuint) glGetUniformLocation(_program_id, "mvp_matrix");
    _texture_loc = (GLuint) glGetUniformLocation(_program_id, "tex");
    _color_loc = (GLuint) glGetUniformLocation(_program_id, "color");

}

void TextObject::preRender(RenderManager const &render_manager)
{
    glUniform1i(_texture_loc, _texture_id);
    glUniform3fv(_color_loc, 1, glm::value_ptr(glm::vec3(0.95f, 0.88f, 0.82f)));
    glUniformMatrix4fv(_mvp_matrix_loc, 1, GL_FALSE, glm::value_ptr(_orthographic_matrix));
//    text = "| H Je . Je suis trop fort yo pouetPOUET loalalalala.";
//    line_pos_x = 0.0f;
//    line_pos_y = 0.0f;
//    scale = 1.0f;
}

void TextObject::render(RenderManager const &render_manager)
{
    glActiveTexture(GL_TEXTURE3);
    glBindVertexArray(_vao);

    GLfloat x = line_pos_x;
    GLfloat y = line_pos_y;

    // Iterate through all characters
    for (std::string::const_iterator it=text.begin(); it!=text.end(); ++it)
    {
        if ((*it) == '\n')
        {
            x = line_pos_x;
            y += max_y;
            continue;
        }

        Character ch = Characters.at(*it);

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = max_y + y - ch.Size.y + (ch.Size.y - ch.Bearing.y);

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 },
            { xpos,     ypos,       0.0, 0.0 },

            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
