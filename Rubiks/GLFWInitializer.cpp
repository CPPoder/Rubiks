#include "GLFWInitializer.hpp"





//Define the WindowSizeChanged Callback Function
void GLFWInitializer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Define MouseCallback Function
void GLFWInitializer::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Set first position
	static bool firstMouse(true);
	static double lastX;
	static double lastY;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	//Calculate mouse offset
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	//Handle mouse movement
	Renderer::camera.ProcessMouseMovement(xOffset, yOffset);
}

//Define Scrool Callback Function
void GLFWInitializer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Renderer::camera.ProcessMouseScroll(yoffset);
}







void GLFWInitializer::Init()
{
	//Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a Window using GLFW
	GLFWwindow* window = glfwCreateWindow(Renderer::width, Renderer::height, Renderer::title.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failded to create GLFW window" << std::endl;
		glfwTerminate();
		throw GLFWInitException();
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Return window
	Renderer::window = window;

	//Set Callback functions
	glfwSetFramebufferSizeCallback(window, GLFWInitializer::framebuffer_size_callback);
	glfwSetCursorPosCallback(window, GLFWInitializer::mouse_callback);
	glfwSetScrollCallback(window, GLFWInitializer::scroll_callback);

}



void GLFWInitializer::DeInit()
{
	glfwTerminate();
	Renderer::window = nullptr;
}


