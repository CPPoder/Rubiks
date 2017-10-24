#include "Renderer.hpp"
#include "GLFWInitializer.hpp"
#include "GLADInitializer.hpp"


GLFWwindow* Renderer::window = nullptr;
unsigned int Renderer::width = 1400u;
unsigned int Renderer::height = 1200u;
std::string Renderer::title = "Title";
USED_CAMERA Renderer::camera = USED_CAMERA();
//glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f





void Renderer::showCube(Cube const & cube)
{
	//Set parameters
	Renderer::width = 800u;
	Renderer::height = 600u;
	Renderer::title = "Cube";
	Renderer::camera = USED_CAMERA(0.f, 0.f, 8.f, 0.f, 1.f, 0.f, -90.f, 0.f);

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

	//FPS Clock
	Clock fpsClock;
	unsigned int fpsCounter = 0u;
	std::chrono::duration<long long, std::micro> fpsDuration = std::chrono::microseconds(0);

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		//Measure FPS
		fpsClock.restart();

		//Determine frametime and sleep to limit FPS
		float frametimeSeconds = static_cast<float>(clock.getElapsedTimeAsMicroseconds()) / 1000000.f;
		std::this_thread::sleep_for(std::chrono::microseconds(1000000 / wantedFPS) - std::chrono::microseconds(clock.getElapsedTimeAsMicroseconds()));
		clock.restart();

		//Process Keyboard Input
		Renderer::processKeyboardInput(Renderer::window, frametimeSeconds);

		//Clear Display
		glClearColor(0.2f, 0.3f, 0.8f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Define Model, View, Projection Matrices
		glm::mat4 modelMatrix;
		glm::mat4 viewMatrix = camera.GetViewMatrix();
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(width) / static_cast<float>(height), 0.1f, 10000.f);

		//Draw
		simpleShader.use();
		simpleShader.setMatrix4f("modelMatrix", modelMatrix);
		simpleShader.setMatrix4f("viewMatrix", viewMatrix);
		simpleShader.setMatrix4f("projectionMatrix", projectionMatrix);
		glBindTexture(GL_TEXTURE_2D, stickerTexture.getID());
		cubeModel.draw(simpleShader);
		glBindVertexArray(0);

		//Display
		glfwSwapBuffers(window);

		//Poll window events
		glfwPollEvents();

		//Measure FPS
		++fpsCounter;
		fpsDuration += std::chrono::microseconds(fpsClock.getElapsedTimeAsMicroseconds());
		if (fpsDuration > std::chrono::microseconds(1000000))
		{
			//std::cout << fpsCounter << std::endl;
			fpsCounter = 0u;
			fpsDuration = std::chrono::microseconds(0);
		}

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
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//{
	//	camera.ProcessKeyboard(USED_CAMERA_MOVEMENT::LEFT, deltaTime);
	//}
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//{
	//	camera.ProcessKeyboard(USED_CAMERA_MOVEMENT::RIGHT, deltaTime);
	//}
	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	//{
	//	camera.ProcessKeyboard(USED_CAMERA_MOVEMENT::UP, deltaTime);
	//}
	//if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	//{
	//	camera.ProcessKeyboard(USED_CAMERA_MOVEMENT::DOWN, deltaTime);
	//}
}




