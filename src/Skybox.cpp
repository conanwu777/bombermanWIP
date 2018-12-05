#include <iostream>
#include "../inc/Skybox.hpp"

Skybox::Skybox()
{
    return ;
}

Skybox::~Skybox()
{
    glDeleteBuffers(2, _vbos);
    glDeleteVertexArrays(1, &_vao);
}

void
Skybox::generateModel(void)
{
    //////////////////////////////////////////////////////////////////
    //                                           					//
    //          4----5		        ^  Y	                     	//
    //         /|   /|              |                     .------.  //
    //       0----1  |              |                    /          //
    //       |  7-|--6              |----->  X          |    <-.    //
    //       |/   | /              /                     \      |   //
    //       3____2               /                        \___/    //
    //                          |,  Z                               //
    //                                           DEFAULT is GL_CCW  //
    //////////////////////////////////////////////////////////////////

    model.v_num_elem = 8 * 3;
    model.f_num_elem = 36;
    model.v_array = std::unique_ptr<GLfloat[]>{ new GLfloat[model.v_num_elem] };
    model.f_array = std::unique_ptr<GLuint[]>{ new GLuint[model.f_num_elem] };
    model.v_mem_size = sizeof(GLfloat) * model.v_num_elem;
    model.f_mem_size = sizeof(GLuint) * model.f_num_elem;

    model.v_array[0] = -1.0f;
    model.v_array[1] = 1.0f;
    model.v_array[2] = 1.0f;

    model.v_array[3] = 1.0f;
    model.v_array[4] = 1.0f;
    model.v_array[5] = 1.0f;

    model.v_array[6] = 1.0f;
    model.v_array[7] = -1.0f;
    model.v_array[8] = 1.0f;

    model.v_array[9] = -1.0f;
    model.v_array[10] = -1.0f;
    model.v_array[11] = 1.0f;

    model.v_array[12] = -1.0f;
    model.v_array[13] = 1.0f;
    model.v_array[14] = -1.0f;

    model.v_array[15] = 1.0f;
    model.v_array[16] = 1.0f;
    model.v_array[17] = -1.0f;

    model.v_array[18] = 1.0f;
    model.v_array[19] = -1.0f;
    model.v_array[20] = -1.0f;

    model.v_array[21] = -1.0f;
    model.v_array[22] = -1.0f;
    model.v_array[23] = -1.0f;

    model.f_array[0] = 0;
    model.f_array[1] = 1;
    model.f_array[2] = 2;

    model.f_array[3] = 0;
    model.f_array[4] = 2;
    model.f_array[5] = 3;

    model.f_array[6] = 1;
    model.f_array[7] = 5;
    model.f_array[8] = 6;

    model.f_array[9] = 1;
    model.f_array[10] = 6;
    model.f_array[11] = 2;

    model.f_array[12] = 5;
    model.f_array[13] = 4;
    model.f_array[14] = 7;

    model.f_array[15] = 5;
    model.f_array[16] = 7;
    model.f_array[17] = 6;

    model.f_array[18] = 4;
    model.f_array[19] = 0;
    model.f_array[20] = 3;

    model.f_array[21] = 4;
    model.f_array[22] = 3;
    model.f_array[23] = 7;

    model.f_array[24] = 1;
    model.f_array[25] = 0;
    model.f_array[26] = 4;

    model.f_array[27] = 1;
    model.f_array[28] = 4;
    model.f_array[29] = 5;

    model.f_array[30] = 6;
    model.f_array[31] = 7;
    model.f_array[32] = 3;

    model.f_array[33] = 6;
    model.f_array[34] = 3;
    model.f_array[35] = 2;
}

void Skybox::print()
{
    std::cerr << "face: ";
    for (GLuint i = 0; i < model.f_num_elem; ++i)
    {
        std::cerr << "[" << model.f_array[i] << "]";
    }
    std::cerr << std::endl;
    std::cerr << "vertex: ";
    for (GLuint i = 0; i < model.v_num_elem; ++i)
    {
        std::cerr << "[" << model.v_array[i] << "]";
    }
    std::cerr << std::endl;

}

void Skybox::initGL(RenderManager const &render_manager)
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(2, _vbos);
    generateModel();
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, model.v_mem_size, model.v_array.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.f_mem_size, model.f_array.get(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(RenderManager::position_loc);
    glVertexAttribPointer(RenderManager::position_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    _program_id = render_manager.getShaderProgramID(SKYBOX_SHADER);
    _texture_id = SKYBOX_TEXTURE;
    _mvp_matrix_loc = (GLuint) glGetUniformLocation(_program_id, "mvp_matrix");
    _texture_loc = (GLuint) glGetUniformLocation(_program_id, "tex");
}

void Skybox::preRender(RenderManager const &render_manager)
{
    glm::mat4	untranslated_view_matrix;

    glUniform1i(_texture_loc, _texture_id);
    /// Convert to mat3 and back to mat4 to remove any translation component.
    untranslated_view_matrix = glm::mat4(glm::mat3(render_manager.getCamera()->view_matrix));
    _vp_matrix = render_manager.getCamera()->perspective_matrix;
    _vp_matrix *= untranslated_view_matrix;
    glUniformMatrix4fv(_mvp_matrix_loc, 1, GL_FALSE, glm::value_ptr(_vp_matrix));
}

void Skybox::render(RenderManager const &render_manager)
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[1]);
    glDepthMask(GL_FALSE);
    glDrawElements(GL_TRIANGLES, model.f_num_elem, GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
}
