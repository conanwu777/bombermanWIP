
#include "../inc/Utils.hpp"
#include <GLM/ext.hpp>
#include <fstream>
#include <iostream>

std::string
getFileContents(std::string const &filename)
{
	std::ifstream		source_file(filename.c_str());

	if (!source_file)
		return (nullptr);

	return std::string(std::istreambuf_iterator<char>(source_file),
						 std::istreambuf_iterator<char>());
}

int
printError(std::ostream &msg, int const &code)
{
	std::cerr << dynamic_cast<std::ostringstream &>(msg).str() << std::endl;
	return (code);
}

int
printError(std::string const &msg, int const &code)
{
	std::cerr << msg << std::endl;
	return (code);
}

inline float
dot(glm::vec3 v1, glm::vec3 v2)
{
	return glm::dot<3, float, glm::qualifier::highp>(v1, v2);
}

bool
equal(glm::vec3 const &v1, glm::vec3 const &v2)
{
	return abs(v1.x - v2.x) < EPSILON && abs(v1.y - v2.y) < EPSILON && abs(v1.z - v2.z) < EPSILON;
}