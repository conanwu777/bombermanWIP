#ifndef OBJOBJECT_HPP
# define OBJOBJECT_HPP

# include <vector>
# include "RenderManager.hpp"
# include "IRenderedObject.hpp"
#include "ObjectLoader.hpp"

class OBJObject : IRenderedObject
{
public:

    std::unique_ptr<ObjectLoader> loader;

	glm::mat4				*linked_matrix;

    OBJObject();
    ~OBJObject();

	void        print();
	void        generateModel();
	void        initGL(RenderManager const &render_manager);
	void        preRender(RenderManager const &render_manager);
	void        render(RenderManager const &render_manager);

protected:

    GLuint                          _vao;
	GLuint							_vbos[2];
};

#endif
