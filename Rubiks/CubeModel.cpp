#include "CubeModel.hpp"

const std::array<glm::vec3, CubeModel::numberOfVertices> CubeModel::arrayOfVertices = 
{
	//Up
	glm::vec3(-1, +1, -1),
	glm::vec3(+0, +1, -1),
	glm::vec3(+1, +1, -1),
	glm::vec3(-1, +1, +0),
	glm::vec3(+0, +1, +0),
	glm::vec3(+1, +1, +0),
	glm::vec3(-1, +1, +1),
	glm::vec3(+0, +1, +1),
	glm::vec3(+1, +1, +1),

	//Down
	glm::vec3(-1, -1, +1),
	glm::vec3(+0, -1, +1),
	glm::vec3(+1, -1, +1),
	glm::vec3(-1, -1, +0),
	glm::vec3(+0, -1, +0),
	glm::vec3(+1, -1, +0),
	glm::vec3(-1, -1, -1),
	glm::vec3(+0, -1, -1),
	glm::vec3(+1, -1, -1),

	//Right
	glm::vec3(+1, +1, +1),
	glm::vec3(+1, +1, +0),
	glm::vec3(+1, +1, -1),
	glm::vec3(+1, +0, +1),
	glm::vec3(+1, +0, +0),
	glm::vec3(+1, +0, -1),
	glm::vec3(+1, -1, +1),
	glm::vec3(+1, -1, +0),
	glm::vec3(+1, -1, -1),

	//Left
	glm::vec3(-1, +1, -1),
	glm::vec3(-1, +1, +0),
	glm::vec3(-1, +1, +1),
	glm::vec3(-1, +0, -1),
	glm::vec3(-1, +0, +0),
	glm::vec3(-1, +0, +1),
	glm::vec3(-1, -1, -1),
	glm::vec3(-1, -1, +0),
	glm::vec3(-1, -1, +1),

	//Front
	glm::vec3(-1, +1, +1),
	glm::vec3(+0, +1, +1),
	glm::vec3(+1, +1, +1),
	glm::vec3(-1, +0, +1),
	glm::vec3(+0, +0, +1),
	glm::vec3(+1, +0, +1),
	glm::vec3(-1, -1, +1),
	glm::vec3(+0, -1, +1),
	glm::vec3(+1, -1, +1),

	//Back
	glm::vec3(+1, +1, -1),
	glm::vec3(+0, +1, -1),
	glm::vec3(-1, +1, -1),
	glm::vec3(+1, +0, -1),
	glm::vec3(+0, +0, -1),
	glm::vec3(-1, +0, -1),
	glm::vec3(+1, -1, -1),
	glm::vec3(+0, -1, -1),
	glm::vec3(-1, -1, -1)
};


const std::array<glm::vec3, CubeModel::numberOfVertices> CubeModel::arrayOfNormals =
{
	//Up
	glm::vec3(+0, +1, +0),
	glm::vec3(+0, +1, +0),
	glm::vec3(+0, +1, +0),
	glm::vec3(+0, +1, +0),
	glm::vec3(+0, +1, +0),
	glm::vec3(+0, +1, +0),
	glm::vec3(+0, +1, +0),
	glm::vec3(+0, +1, +0),
	glm::vec3(+0, +1, +0),

	//Down
	glm::vec3(+0, -1, +0),
	glm::vec3(+0, -1, +0),
	glm::vec3(+0, -1, +0),
	glm::vec3(+0, -1, +0),
	glm::vec3(+0, -1, +0),
	glm::vec3(+0, -1, +0),
	glm::vec3(+0, -1, +0),
	glm::vec3(+0, -1, +0),
	glm::vec3(+0, -1, +0),

	//Right
	glm::vec3(+1, +0, +0),
	glm::vec3(+1, +0, +0),
	glm::vec3(+1, +0, +0),
	glm::vec3(+1, +0, +0),
	glm::vec3(+1, +0, +0),
	glm::vec3(+1, +0, +0),
	glm::vec3(+1, +0, +0),
	glm::vec3(+1, +0, +0),
	glm::vec3(+1, +0, +0),

	//Left
	glm::vec3(-1, +0, +0),
	glm::vec3(-1, +0, +0),
	glm::vec3(-1, +0, +0),
	glm::vec3(-1, +0, +0),
	glm::vec3(-1, +0, +0),
	glm::vec3(-1, +0, +0),
	glm::vec3(-1, +0, +0),
	glm::vec3(-1, +0, +0),
	glm::vec3(-1, +0, +0),

	//Front
	glm::vec3(+0, +0, +1),
	glm::vec3(+0, +0, +1),
	glm::vec3(+0, +0, +1),
	glm::vec3(+0, +0, +1),
	glm::vec3(+0, +0, +1),
	glm::vec3(+0, +0, +1),
	glm::vec3(+0, +0, +1),
	glm::vec3(+0, +0, +1),
	glm::vec3(+0, +0, +1),

	//Back
	glm::vec3(+0, +0, -1),
	glm::vec3(+0, +0, -1),
	glm::vec3(+0, +0, -1),
	glm::vec3(+0, +0, -1),
	glm::vec3(+0, +0, -1),
	glm::vec3(+0, +0, -1),
	glm::vec3(+0, +0, -1),
	glm::vec3(+0, +0, -1),
	glm::vec3(+0, +0, -1)
};



CubeModel::CubeModel(Cube const & cube)
{
	std::vector<Color> colorVector = CubeModel::getVectorOfColorsFromCube(cube);

	//Create vbo data
	float vboData[CubeModel::numberOfVertices * 7]; //7 = 3 + 3 + 1 (pos + normal + color)
	for (unsigned int i = 0; i < CubeModel::arrayOfVertices.size(); ++i)
	{
		vboData[7 * i + 0] = CubeModel::arrayOfVertices.at(i).x;
		vboData[7 * i + 1] = CubeModel::arrayOfVertices.at(i).y;
		vboData[7 * i + 2] = CubeModel::arrayOfVertices.at(i).z;
		vboData[7 * i + 3] = CubeModel::arrayOfNormals.at(i).x;
		vboData[7 * i + 4] = CubeModel::arrayOfNormals.at(i).y;
		vboData[7 * i + 5] = CubeModel::arrayOfNormals.at(i).z;
		vboData[7 * i + 6] = CubeModel::mapColorToFloat(colorVector.at(i));
	}


	//Do actual OpenGL stuff
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


CubeModel::~CubeModel()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
}


void CubeModel::draw(Shader const & shader)
{
	shader.use();
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, CubeModel::numberOfVertices);
	glBindVertexArray(0);
}




std::vector<Color> CubeModel::getVectorOfColorsFromCube(Cube const & cube)
{
	std::function<void(std::vector<Color> &, std::array<Color, 8u> const &, Color)> fillVecWithArray = [](std::vector<Color> & v, std::array<Color, 8u> const & a, Color c)
	{
		v.push_back(a.at(0));
		v.push_back(a.at(1));
		v.push_back(a.at(2));
		v.push_back(a.at(3));
		v.push_back(c);
		v.push_back(a.at(4));
		v.push_back(a.at(5));
		v.push_back(a.at(6));
		v.push_back(a.at(7));
	};

	std::array<Color, 8u> upArray = cube.at(PlanePos::UP).getArrayOfRowwiseColors();
	std::array<Color, 8u> downArray = cube.at(PlanePos::DOWN).getArrayOfRowwiseColors();
	std::array<Color, 8u> rightArray = cube.at(PlanePos::RIGHT).getArrayOfRowwiseColors();
	std::array<Color, 8u> leftArray = cube.at(PlanePos::LEFT).getArrayOfRowwiseColors();
	std::array<Color, 8u> frontArray = cube.at(PlanePos::FRONT).getArrayOfRowwiseColors();
	std::array<Color, 8u> backArray = cube.at(PlanePos::BACK).getArrayOfRowwiseColors();

	std::vector<Color> colorVector;

	fillVecWithArray(colorVector, upArray, Color::WHITE);
	fillVecWithArray(colorVector, downArray, Color::YELLOW);
	fillVecWithArray(colorVector, rightArray, Color::BLUE);
	fillVecWithArray(colorVector, leftArray, Color::GREEN);
	fillVecWithArray(colorVector, frontArray, Color::RED);
	fillVecWithArray(colorVector, backArray, Color::ORANGE);

	return std::move(colorVector);
}

float CubeModel::mapColorToFloat(Color const & color)
{
	switch (color)
	{
	case Color::WHITE:
		return 1.0;
	case Color::YELLOW:
		return 2.0;
	case Color::BLUE:
		return 3.0;
	case Color::GREEN:
		return 4.0;
	case Color::RED:
		return 5.0;
	case Color::ORANGE:
		return 6.0;
	}
}



