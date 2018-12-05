#ifndef UTILS_HPP
# define UTILS_HPP

# include <GLM/vec3.hpp>
# include <string>
# include <sstream>

# define EPSILON 0.00001f

int					printError(std::ostream &msg, int const &code);
int					printError(std::string const &msg, int const &code);
std::string			getFileContents(std::string const &filename);
float               dot(glm::vec3 v1, glm::vec3 v2);
bool                equal(glm::vec3 const &v1, glm::vec3 const &v2);

#endif