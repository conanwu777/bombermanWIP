#include <iostream>
#include "../inc/Axes.hpp"

Axes::Axes()
{
}

Axes::~Axes()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void Axes::print()
{
    std::cerr << "vertex: ";
    for (GLuint i = 0; i < model.v_num_elem; ++i)
    {
        std::cerr << "[" << model.v_array[i] << "]";
    }
    std::cerr << std::endl;
}

void
Axes::generateModel()
{
    GLfloat axes_rgb_vertex_and_color[216] = {
            //Red +
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            100.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, 0.01f, 0.0f,
            1.0f, 0.0f, 0.0f,
            100.0f, 0.01f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, -0.01f, 0.0f,
            1.0f, 0.0f, 0.0f,
            100.0f, -0.01f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.01f,
            1.0f, 0.0f, 0.0f,
            100.0f, 0.0f, 0.1f,
            1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, -0.01f,
            1.0f, 0.0f, 0.0f,
            100.0f, 0.0f, -0.1f,
            1.0f, 0.0f, 0.0f,

            //Green +
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 100.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            0.01f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.01f, 100.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            -0.01f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            -0.01f, 100.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, 0.01f,
            0.0f, 1.0f, 0.0f,
            0.0f, 100.0f, 0.01f,
            0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, -0.01f,
            0.0f, 1.0f, 0.0f,
            0.0f, 100.0f, -0.01f,
            0.0f, 1.0f, 0.0f,

            //Blue +
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 100.0f,
            0.0f, 0.0f, 1.0f,

            0.01f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.01f, 0.0f, 100.0f,
            0.0f, 0.0f, 1.0f,

            -0.01f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            -0.01f, 0.0f, 100.0f,
            0.0f, 0.0f, 1.0f,

            0.0f, 0.01f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.01f, 100.0f,
            0.0f, 0.0f, 1.0f,

            0.0f, -0.01f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, -0.01f, 100.0f,
            0.0f, 0.0f, 1.0f,

            // Red -
            0.0f, 0.0f, 0.0f,
            0.7f, 0.0f, 0.0f,
            -100.0f, 0.0f, 0.0f,
            0.7f, 0.0f, 0.0f,

            // Green -
            0.0f, 0.0f, 0.0f,
            0.0f, 0.7f, 0.0f,
            0.0f, -100.0f, 0.0f,
            0.0f, 0.7f, 0.0f,

            // Blue -
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.7f,
            0.0f, 0.0f, -100.0f,
            0.0f, 0.0f, 0.7f,
    };
    model.v_num_elem = 36;
    model.v_mem_size = sizeof(GLfloat) * 216;
    model.v_array = std::unique_ptr<GLfloat[]>{new GLfloat[216]};
    for (int i = 0; i < 216; ++i)
        model.v_array[i] = axes_rgb_vertex_and_color[i];
}

void Axes::initGL(RenderManager const &render_manager)
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    generateModel();
    glBufferData(GL_ARRAY_BUFFER, model.v_mem_size, model.v_array.get(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(RenderManager::position_loc);
    glVertexAttribPointer(RenderManager::position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, nullptr);
    glEnableVertexAttribArray(RenderManager::color_loc);
    glVertexAttribPointer(RenderManager::color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
                          (void *) (3 * sizeof(GLfloat)));

    _program_id = render_manager.getShaderProgramID(AXES_SHADER);
    _mvp_matrix_loc = (GLuint) glGetUniformLocation(_program_id, "mvp_matrix");
}

void Axes::preRender(RenderManager const &render_manager)
{
    if (render_manager.render_axes_on)
    {
        _vp_matrix = render_manager.getCamera()->perspective_matrix;
        _vp_matrix *= render_manager.getCamera()->view_matrix;
        glUniformMatrix4fv(_mvp_matrix_loc, 1, GL_FALSE, glm::value_ptr(_vp_matrix));
    }
}

void Axes::render(RenderManager const &render_manager)
{
    if (render_manager.render_axes_on)
    {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glDrawArrays(GL_LINES, 0, model.v_num_elem);
    }
}
