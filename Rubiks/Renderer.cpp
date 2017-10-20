#include "Renderer.hpp"
#include "GLFWInitializer.hpp"
#include "GLADInitializer.hpp"


GLFWwindow* Renderer::window = nullptr;
unsigned int Renderer::width = 1400u;
unsigned int Renderer::height = 1200u;
std::string Renderer::title = "Title";
Camera Renderer::camera = Camera();





void Renderer::showCube(Cube const & cube)
{
	//Set parameters
	Renderer::width = 800u;
	Renderer::height = 600u;
	Renderer::title = "Cube";
	Renderer::camera = Camera();

	//Initialize Renderer
	Renderer::Init();

	//Create Shader
	Shader simpleShader("./Data/Shader/Simple.vs", "./Data/Shader/Simple.fs");

	//Create model data
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f,  
		0.5f,  0.5f, 0.0f  
	};

	unsigned int indices[] =
	{
		0, 1, 2
	};

	//Create VBO, EBO and VAO
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		Renderer::processKeyboardInput(Renderer::window, 0.0f);

		glClearColor(0.2f, 0.3f, 0.8f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		simpleShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//DeInitialize Renderer
	Renderer::DeInit();
}



void Renderer::Init()
{
	GLFWInitializer::Init();
	GLADInitializer::Init();
	glEnable(GL_DEPTH_TEST);
}



void Renderer::DeInit()
{
	GLADInitializer::DeInit();
	GLFWInitializer::DeInit();
}





void Renderer::processKeyboardInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}




