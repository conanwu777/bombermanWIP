#include "../inc/ShaderManager.hpp"

ShaderManager::ShaderManager()
{
}

ShaderManager::ShaderManager(ShaderManager const &src)
{
	*this = src;
}

ShaderManager::~ShaderManager()
{
	std::cerr << "ShaderManager() destructor called." << std::endl;
}

bool
ShaderManager::read(const GLchar *vertex_shader, const GLchar *geometry_shader, const GLchar *fragment_shader)
{
	bool success = true;

	if (vertex_shader != nullptr)
	{
		if (!(vs_presence = readShader(GL_VERTEX_SHADER, vertex_shader, &this->vertex_shader)))
		{
			std::cerr << "Vertex shader failed to load." << std::endl;
			success = false;
		}
	}
	if (geometry_shader != nullptr)
	{
		if (!(gs_presence = readShader(GL_GEOMETRY_SHADER, geometry_shader, &this->geometry_shader)))
		{
			std::cerr << "Geometry shader failed to load." << std::endl;
			success = false;
		}
	}
	if (fragment_shader != nullptr)
	{
		if (!(fs_presence = readShader(GL_FRAGMENT_SHADER, fragment_shader, &this->fragment_shader)))
		{
			std::cerr << "Fragment shader failed to load." << std::endl;
			success = false;
		}
	}

	return success;
}

bool
ShaderManager::readShader(GLenum type, std::string file_name, GLuint *current_shader)
{
	std::ifstream		source_file(file_name.c_str());
	std::string			shader_string;
	const GLchar		*shader_source;

	if (!source_file)
		return (false);

	shader_string.assign(std::istreambuf_iterator<char>(source_file),
						 std::istreambuf_iterator<char>());
	*current_shader = glCreateShader(type);

	shader_source = shader_string.c_str();
	glShaderSource(*current_shader, 1, &shader_source, NULL);

	return (true);
}

void
ShaderManager::compile()
{
    if (vs_presence)
    {
        if (!compileShader(&this->vertex_shader))
            std::cerr << "Vertex shader failed to compile." << std::endl;
    }
    if (gs_presence)
    {
        if (!compileShader(&this->geometry_shader))
            std::cerr << "Geometry shader failded to compile." << std::endl;
    }
    if (fs_presence)
    {
        if (!compileShader(&this->fragment_shader))
            std::cerr << "Fragment shader failed to compile." << std::endl;
    }
}

bool
ShaderManager::compileShader(const GLuint *current_shader)
{
	GLint		is_compiled;

	is_compiled = 0;
	glCompileShader(*current_shader);
	glGetShaderiv(*current_shader, GL_COMPILE_STATUS, &is_compiled);
	if (is_compiled == GL_FALSE)
	{
		GLint	max_length = 0;
		glGetShaderiv(*current_shader, GL_INFO_LOG_LENGTH, &max_length);
		std::vector<GLchar> comp_log(max_length);
		glGetShaderInfoLog(*current_shader, max_length, &max_length, &comp_log[0]);
		for (std::vector<char>::const_iterator i = comp_log.begin() ; i != comp_log.end() ; ++i)
		{
			std::cerr << *i ;
		}
		std::cerr << std::endl;
		glDeleteShader(*current_shader);
		return (false);
	}
	return (true);
}

bool
ShaderManager::linkProgram(const GLuint *program)
{
	GLint		is_linked;

	is_linked = 0;
    if (vs_presence)
	    glAttachShader(*program, this->vertex_shader);
    if (gs_presence)
        glAttachShader(*program, this->geometry_shader);
    if (fs_presence)
	    glAttachShader(*program, this->fragment_shader);
	glLinkProgram(*program);
	glGetProgramiv(*program, GL_LINK_STATUS, &is_linked);
	if (is_linked == GL_FALSE)
	{
		GLint	max_length = 0;
		glGetShaderiv(*program, GL_INFO_LOG_LENGTH, &max_length);
		std::vector<GLchar> link_log(max_length);
		glGetShaderInfoLog(*program, max_length, &max_length, &link_log[0]);
		for (std::vector<char>::const_iterator i = link_log.begin() ; i != link_log.end() ; ++i)
		{
			std::cerr << *i ;
		}
		std::cerr << std::endl;
		glDeleteProgram(*program);
        if (vs_presence)
		    glDeleteShader(this->vertex_shader);
        if (gs_presence)
            glDeleteShader(this->geometry_shader);
        if (fs_presence)
            glDeleteShader(this->fragment_shader);
        return (false);
	}
    if (vs_presence)
	    glDetachShader(*program, this->vertex_shader);
    if (gs_presence)
        glDetachShader(*program, this->geometry_shader);
    if (fs_presence)
	    glDetachShader(*program, this->fragment_shader);
	return (true);
}

ShaderManager
&ShaderManager::operator=(ShaderManager const &rhs)
{
	this->vertex_shader = rhs.vertex_shader;
    this->geometry_shader = rhs.geometry_shader;
	this->fragment_shader = rhs.fragment_shader;
	return *this;
}
