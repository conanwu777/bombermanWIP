//
// Created by Remi DAVID on 10/5/18.
//

#ifndef I_RENDERED_OBJECT_HPP
# define I_RENDERED_OBJECT_HPP

# include <GLM/glm.hpp>

# if defined(_WIN32)
#  include <GL/glew.h>
# else
#  include <OpenGL/OpenGL.h>
# endif

class RenderManager;

class IRenderedObject
{
public:

    virtual void initGL(RenderManager const &render_manager) = 0;
    virtual void preRender(RenderManager const &render_manager) = 0;
    virtual void render(RenderManager const &render_manager) = 0;

    GLuint const &getProgramID() const;

protected:

    glm::mat4       _vp_matrix = glm::mat4(1.0f);
    glm::mat4       _m_matrix = glm::mat4(1.0f);
    glm::mat4       _mvp_matrix = glm::mat4(1.0f);

    GLuint          _program_id = 0;
    GLuint          _texture_id = 0;

    GLuint          _camera_pos_loc = 0;
    GLuint          _light_pos_loc = 0;
    GLuint          _texture_loc = 0;
    GLuint          _time_loc = 0;

    GLuint          _mvp_matrix_loc = 0;
    GLuint          _model_loc = 0;
};

#endif //IRENDERED_OBJECT_HPP
