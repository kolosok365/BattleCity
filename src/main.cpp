#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"

//Шейдер - это определяемая пользователем программа, предназначенная для запуска на некотором этапе графического процессора.
//Bind - связывать

glm::ivec2 g_windowSize(640, 480);

Game g_game(g_windowSize);




void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
	g_windowSize.x = width;
	g_windowSize.y = height;
	glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}
	
	g_game.setKey(key, action);
}

int main(int argc, char** argv)
{

	/* Initialize the library */
	if (!glfwInit()) {//Библиотека инициализируется с помощью glfwInit, который возвращает GLFW_FALSE в случае возникновения ошибки.
		std::cout << "glfwInit failed!";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//Настройка GLFW. Задается минимальная требуемая версия OpenGL. Мажорная.
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

	if (!gladLoadGL()) { //инициализация библиотеки GLAD
		std::cout << "Can't load GLAD!" << std::endl;
		return -1;
	}

	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glClearColor(0, 0, 0, 1);

	{
		ResourceManager::setExecutablePath(argv[0]);
		g_game.init();
		
		auto lastTime = std::chrono::high_resolution_clock::now();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(pWindow))//проверка флага должно ли быть закрыто окно
		{
			
			auto currentTime = std::chrono::high_resolution_clock::now();
			uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
			lastTime = currentTime;
			g_game.update(duration);
			

			/* Render here*/
			glClear(GL_COLOR_BUFFER_BIT);//очищает буферы до заданных значений

			g_game.render();

			/* Swap front and back buffers */
			glfwSwapBuffers(pWindow);//функция меняет местами передний и задний буферы указанного окна. 

			/* Poll for and process events */
			glfwPollEvents();//обработка всех событий
		}
	}

	ResourceManager::unloadAllResources();
}