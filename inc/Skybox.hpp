#ifndef SKYBOX_HPP
# define SKYBOX_HPP

# include <vector>
#include <memory>
# include "RenderManager.hpp"
# include "IRenderedObject.hpp"

class Skybox : IRenderedObject
{
public:

    struct ModelData
    {
        GLuint			           v_mem_size;
        GLuint			           v_num_elem;
        std::unique_ptr<GLfloat[]> v_array;
        GLuint			           f_mem_size;
        GLuint			           f_num_elem;
        std::unique_ptr<GLuint[]>  f_array;
    };

    ModelData       model;

    Skybox(void);
    ~Skybox(void);

    void        initGL(RenderManager const &render_manager);
    void        preRender(RenderManager const &render_manager);
    void        render(RenderManager const &render_manager);
    void        generateModel(void);
    void        print(void);

protected:

    GLuint                          _vao;
	GLuint							_vbos[2];
};

#endif
