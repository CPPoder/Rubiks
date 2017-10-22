#ifndef SHADER_HPP
#define SHADER_HPP


#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	GLuint ID;

	Shader() = default;
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
	~Shader();

	void use() const;

	void setBool(const std::string & name, bool value) const;
	void setInt(const std::string & name, int value) const;
	void setFloat(const std::string & name, float value) const;
	void setMatrix4f(const std::string & name, glm::mat4 const & matrix) const;
	void setMatrix3f(const std::string & name, glm::mat3 const & matrix) const;
	void setVector3f(const std::string & name, glm::vec3 const & vector) const;

};





#endif //SHADER_HPP
