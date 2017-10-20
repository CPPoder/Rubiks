#ifndef GLFWINITIALIZER_HPP
#define GLFWINITIALIZER_HPP

#include "GLIncludes.hpp"

#include "Renderer.hpp"


class GLFWInitException{};

class GLFWInitializer
{
public:
	static void Init();
	static void DeInit();


private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};






#endif //GLFWINITIALIZER_HPP

