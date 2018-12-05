#ifndef SHADERS_HPP
# define SHADERS_HPP

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

# if defined(_WIN32)
#  include <GL/glew.h>
# else
#  include <OpenGL/OpenGL.h>
# endif

# include <GLFW/glfw3.h>
# include <iostream>
# include <fstream>
# include <string>
# include <vector>

class ShaderManager
{

public:
	GLuint		vertex_shader;
	GLuint		geometry_shader;
	GLuint		fragment_shader;

	bool		vs_presence = false;
	bool		gs_presence = false;
	bool		fs_presence = false;

	ShaderManager();
	ShaderManager(ShaderManager const &src);
	~ShaderManager();

	bool		read(const GLchar *vertex_shader, const GLchar *geometry_shader, const GLchar *fragment_shader);
	bool		readShader(GLenum type, std::string file_name, GLuint *current_shader);

	void		compile();
	bool		compileShader(const GLuint *current_shader);

	bool		linkProgram(const GLuint *program);

	ShaderManager		&operator=(ShaderManager const &rhs);
};

#endif
