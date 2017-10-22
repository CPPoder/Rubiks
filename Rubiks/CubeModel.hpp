#ifndef CUBEMODEL_HPP
#define CUBEMODEL_HPP

#include "GLIncludes.hpp"

#include "Shader.hpp"
#include "Cube.hpp"

#include <array>


class CubeModel
{
public:
	static constexpr unsigned int numberOfVertices = 6u * 9u; //numPlanes * numSticker

private:
	GLuint vbo = 0u;
	GLuint vao = 0u;

public:
	CubeModel(Cube const & cube);
	CubeModel(CubeModel const & cubeModel) = delete;
	CubeModel& operator=(CubeModel const & cubeModel) = delete;
	~CubeModel();

public:
	void draw(Shader const & shader);

private:
	static const std::array<glm::vec3, CubeModel::numberOfVertices> arrayOfVertices;
	static const std::array<glm::vec3, CubeModel::numberOfVertices> arrayOfNormals;
	
private:
	static std::vector<Color> getVectorOfColorsFromCube(Cube const & cube);
	static float mapColorToFloat(Color const & color);
};





#endif //CUBEMODEL_HPP

