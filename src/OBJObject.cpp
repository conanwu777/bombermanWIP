#include <iostream>
#include "../inc/OBJObject.hpp"

OBJObject::OBJObject()
{
}

OBJObject::~OBJObject()
{
    glDeleteBuffers(2, _vbos);
    glDeleteVertexArrays(1, &_vao);
}

void OBJObject::print()
{
    std::cerr << "vertex: ";
    for (GLuint i = 0; i < loader->iid.num_elem; ++i)
    {
        std::cerr << "[" << loader->iid.array[i] << "]";
    }
    std::cerr << std::endl;
}

void
OBJObject::generateModel()
{
    loader = std::unique_ptr<ObjectLoader>{ new ObjectLoader("../assets/obj/tree.obj") };
    loader->makeOpenGLBuffer();
}

void OBJObject::initGL(RenderManager const &render_manager)
{
    generateModel();
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(2, _vbos);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, loader->iid.mem_size, loader->iid.array.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader->iid.index_mem_size, loader->iid.index.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(RenderManager::position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, nullptr);
    glEnableVertexAttribArray(RenderManager::position_loc);
    glVertexAttribPointer(RenderManager::UV_loc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(RenderManager::UV_loc);
    glVertexAttribPointer(RenderManager::normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *) (5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(RenderManager::normal_loc);

    _program_id = render_manager.getShaderProgramID(OBJ_SHADER);
    _texture_id = CHECKER_TEXTURE;
    _texture_loc = (GLuint) glGetUniformLocation(_program_id, "tex");
    _mvp_matrix_loc = (GLuint) glGetUniformLocation(_program_id, "mvp_matrix");
    _model_loc = (GLuint) glGetUniformLocation(_program_id, "model_matrix");
}

void OBJObject::preRender(RenderManager const &render_manager)
{
    glUniform1i(_texture_loc, _texture_id);
    _mvp_matrix = render_manager.getCamera()->perspective_matrix;
    _mvp_matrix *= render_manager.getCamera()->view_matrix;
    _mvp_matrix *= *linked_matrix;
    glUniformMatrix4fv(_mvp_matrix_loc, 1, GL_FALSE, glm::value_ptr(_mvp_matrix));
    glUniformMatrix4fv(_model_loc, 1, GL_FALSE, glm::value_ptr(_m_matrix));
}

void OBJObject::render(RenderManager const &render_manager)
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[1]);
    glDrawElements(GL_TRIANGLES, loader->iid.num_elem, GL_UNSIGNED_INT, nullptr);
}
