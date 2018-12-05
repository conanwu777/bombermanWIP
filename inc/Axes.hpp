#ifndef AXES_HPP
# define AXES_HPP

# include <vector>
#include <memory>
# include "RenderManager.hpp"
# include "IRenderedObject.hpp"

class Axes : IRenderedObject
{
public:

    struct ModelData
    {
        GLuint			           v_mem_size;
        GLuint		               v_num_elem;
        std::unique_ptr<GLfloat[]> v_array;
    };

    ModelData       model;

    Axes(void);
    ~Axes(void);

    void        initGL(RenderManager const &render_manager);
    void        preRender(RenderManager const &render_manager);
    void        render(RenderManager const &render_manager);
    void        generateModel(void);
    void        print(void);

protected:

    GLuint                          _vao;
	GLuint							_vbo;
};

#endif
