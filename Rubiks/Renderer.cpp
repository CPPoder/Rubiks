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
	Shader simpleShader("./Data/Shader/Cube.vs", "./Data/Shader/Cube.fs", "./Data/Shader/Cube.gs");

	//Create Model
	CubeModel cubeModel(cube);

	//Create Texture
	my::Texture stickerTexture;
	stickerTexture.load("./Data/Textures/emissionMapQuadratic512.png", true, GL_RGBA);

	//Create Clock
	Clock clock;
	clock.restart();

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		//Determine frametime
		float frametimeSeconds = static_cast<float>(clock.getElapsedTimeAsMicroseconds()) / 1000000.f;
		clock.restart();

		Renderer::processKeyboardInput(Renderer::window, frametimeSeconds);

		glClearColor(0.2f, 0.3f, 0.8f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -5.f));

		glm::mat4 viewMatrix = camera.GetViewMatrix();

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(width) / static_cast<float>(height), 0.1f, 10000.f);

		simpleShader.use();
		simpleShader.setMatrix4f("modelMatrix", modelMatrix);
		simpleShader.setMatrix4f("viewMatrix", viewMatrix);
		simpleShader.setMatrix4f("projectionMatrix", projectionMatrix);
		glBindTexture(GL_TEXTURE_2D, stickerTexture.getID());
		cubeModel.draw(simpleShader);
		
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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
}




