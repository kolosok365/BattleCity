#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"

//������ - ��� ������������ ������������� ���������, ��������������� ��� ������� �� ��������� ����� ������������ ����������.
//Bind - ���������

glm::ivec2 g_windowSize(13 * 16, 14 * 16);

std::unique_ptr<Game> g_game = std::make_unique<Game>(g_windowSize);




void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
	g_windowSize.x = width;
	g_windowSize.y = height;

	const float map_aspect_ratio = static_cast<float>(g_game->getCurrentLevelWidth() / g_game->getCurrentLevelHeight());
	
	unsigned int viewPortWidth = g_windowSize.x;
	unsigned int viewPortHeight = g_windowSize.y;
	unsigned int viewPortLeftOffset = 0;
	unsigned int viewPortBottomOffset = 0;

	if ((static_cast<float>(g_windowSize.x) / g_windowSize.y) > map_aspect_ratio) {
		viewPortWidth = static_cast<unsigned int>(g_windowSize.y * map_aspect_ratio);
		viewPortLeftOffset = static_cast<unsigned int>((g_windowSize.x - viewPortWidth) / 2);
	}
	else {
		viewPortHeight = g_windowSize.x * map_aspect_ratio;
		viewPortBottomOffset = (g_windowSize.y - viewPortHeight) / 2;
	}

	RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}
	
	g_game->setKey(key, action);
}



int main(int argc, char** argv)
{
	
	/* Initialize the library */
	if (!glfwInit()) {//���������� ���������������� � ������� glfwInit, ������� ���������� GLFW_FALSE � ������ ������������� ������.
		std::cout << "glfwInit failed!";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//��������� GLFW. �������� ����������� ��������� ������ OpenGL. ��������.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Battle-CIty", nullptr, nullptr);
	if (!pWindow)
	{
		std::cout << "glfwCreateWindow failed!\n";
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
	glfwSetKeyCallback(pWindow, glfwKeyCallback);

	/* Make the window's context current */
	glfwMakeContextCurrent(pWindow);

	if (!gladLoadGL()) { //������������� ���������� GLAD
		std::cout << "Can't load GLAD!" << std::endl;
		return -1;
	}

	std::cout << "Renderer: " << RenderEngine::Renderer::getRendererStr() << std::endl;
	std::cout << "OpenGL version: " << RenderEngine::Renderer::getVersionStr() << std::endl;

	RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
	RenderEngine::Renderer::setDepthTest(true);

	{
		ResourceManager::setExecutablePath(argv[0]);
		g_game->init();
		glfwSetWindowSize(pWindow, static_cast<int>(3 * g_game->getCurrentLevelWidth()), static_cast<int>(3 * g_game->getCurrentLevelHeight()));
		
		auto lastTime = std::chrono::high_resolution_clock::now();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(pWindow))//�������� ����� ������ �� ���� ������� ����
		{
			/* Poll for and process events */
			glfwPollEvents();//��������� ���� �������

			auto currentTime = std::chrono::high_resolution_clock::now();
			double duration = std::chrono::duration<double,std::milli>(currentTime - lastTime).count();
			lastTime = currentTime;
			g_game->update(duration);
			

			/* Render here*/
			RenderEngine::Renderer::clear();
			g_game->render();

			/* Swap front and back buffers */
			glfwSwapBuffers(pWindow);//������� ������ ������� �������� � ������ ������ ���������� ����. 
		}
		g_game = nullptr;
		ResourceManager::unloadAllResources();
	}

	glfwTerminate();
	return 0;

}