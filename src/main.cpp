#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"

GLfloat point[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

//Шейдер - это определяемая пользователем программа, предназначенная для запуска на некотором этапе графического процессора.
//Bind - связывать

int g_WIndowSizeX = 640;
int g_WIndowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
	g_WIndowSizeX = width;
	g_WIndowSizeY = height;
	glViewport(0, 0, g_WIndowSizeX, g_WIndowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}
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
	GLFWwindow* pWindow = glfwCreateWindow(g_WIndowSizeX, g_WIndowSizeY, "Battle-CIty", nullptr, nullptr);
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

	glClearColor(1, 1, 0, 1);

	{
		ResourceManager resourceManager(argv[0]);
		auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
		if (!pDefaultShaderProgram) {
			std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
			return -1;
		}

		//std::string vertexShader;// (vertex_shader);
		//std::string fragmentShader;// (fragment_shader);
		//Renderer::ShaderProgram shaderProgram(vertexShader, fragmentShader);
		//if (!shaderProgram.isCompiled()) {
		//	std::cerr << "Can't create shader program!" << std::endl;
		//	return -1;
		//}

		GLuint points_vbo = 0;//позиции vertex-a
		glGenBuffers(1, &points_vbo);//драйвер, при вызове команды генерирует один vertex_buffer_object и записывает значение идентификатора указателю points_vbo
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);//подключили и сделали текущим созданный буфер
		glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

		GLuint colors_vbo = 0;//цвета vertex-a
		glGenBuffers(1, &colors_vbo);//драйвер, при вызове команды генерирует один vertex_buffer_object и записывает значение идентификатора указателю points_vbo
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);//подключили и сделали текущим созданный буфер
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		GLuint vao = 0;//vertex_array_object
		glGenVertexArrays(1, &vao);//
		glBindVertexArray(vao);//сделали текущим

		//включение позиции для координат (location = 0)
		glEnableVertexAttribArray(0);//функция включает нулевую позицию в шейдере
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);//подключаем текущий буфер
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);//связываем данные

		//включение позиции для цвета (location = 1)
		glEnableVertexAttribArray(1);//функция включает 1-ую позицию в шейдере
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);//подключаем текущий буфер
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);//связываем данные



		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(pWindow))//проверка флага должно ли быть закрыто окно
		{
			/* Render here*/
			glClear(GL_COLOR_BUFFER_BIT);//очищает буферы до заданных значений

			pDefaultShaderProgram->use();
			//делаем отрисовку
			//glUseProgram(shader_program);//подключение шейдера

			glBindVertexArray(vao);//подключаем vertex_atribut_object, который хотим отрисовать
			glDrawArrays(GL_TRIANGLES, 0, 3);//отрисовка

			/* Swap front and back buffers */
			glfwSwapBuffers(pWindow);//функция меняет местами передний и задний буферы указанного окна. 

			/* Poll for and process events */
			glfwPollEvents();//обработка всех событий
		}
	}

	glfwTerminate();//освобождение всех ресурсов
	return 0;
}