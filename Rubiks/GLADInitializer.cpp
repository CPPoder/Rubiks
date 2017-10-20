#include "GLADInitializer.hpp"



void GLADInitializer::Init()
{
	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw GLADInitException();
	}

	//Set the OpenGL Viewport
	glViewport(0, 0, Renderer::width, Renderer::height);
}



void GLADInitializer::DeInit()
{

}


