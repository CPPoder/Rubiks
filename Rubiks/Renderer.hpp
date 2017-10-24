#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <thread>

#include "Cube.hpp"

#include "Camera.hpp"
#include "OriginCamera.hpp"
#include "Shader.hpp"
#include "CubeModel.hpp"
#include "Clock.hpp"
#include "Texture.hpp"

#include "GLIncludes.hpp"

class GLFWInitializer;
class GLADInitializer;

using USED_CAMERA = OriginCamera;
using USED_CAMERA_MOVEMENT = OriginCameraMovement;

class Renderer
{
private:
	static GLFWwindow* window;
	static unsigned int width;
	static unsigned int height;
	static std::string title;
	static USED_CAMERA camera;
	static constexpr unsigned int wantedFPS = 60u;


public:
	Renderer() = delete;
	Renderer(Renderer const &) = delete;
	~Renderer() = delete;
	Renderer& operator=(Renderer const &) = delete;

public:
	static void showCube(Cube const & cube);


private:
	static void Init();
	static void DeInit();

	static void processKeyboardInput(GLFWwindow* window, float deltaTime);



	friend class GLFWInitializer;
	friend class GLADInitializer;


};





#endif //RENDERER_HPP

