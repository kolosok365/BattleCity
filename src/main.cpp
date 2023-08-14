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

//������ - ��� ������������ ������������� ���������, ��������������� ��� ������� �� ��������� ����� ������������ ����������.
//Bind - ���������

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
	if (!glfwInit()) {//���������� ���������������� � ������� glfwInit, ������� ���������� GLFW_FALSE � ������ ������������� ������.
		std::cout << "glfwInit failed!";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//��������� GLFW. �������� ����������� ��������� ������ OpenGL. ��������.
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

	if (!gladLoadGL()) { //������������� ���������� GLAD
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

		GLuint points_vbo = 0;//������� vertex-a
		glGenBuffers(1, &points_vbo);//�������, ��� ������ ������� ���������� ���� vertex_buffer_object � ���������� �������� �������������� ��������� points_vbo
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);//���������� � ������� ������� ��������� �����
		glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

		GLuint colors_vbo = 0;//����� vertex-a
		glGenBuffers(1, &colors_vbo);//�������, ��� ������ ������� ���������� ���� vertex_buffer_object � ���������� �������� �������������� ��������� points_vbo
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);//���������� � ������� ������� ��������� �����
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		GLuint vao = 0;//vertex_array_object
		glGenVertexArrays(1, &vao);//
		glBindVertexArray(vao);//������� �������

		//��������� ������� ��� ��������� (location = 0)
		glEnableVertexAttribArray(0);//������� �������� ������� ������� � �������
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);//���������� ������� �����
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);//��������� ������

		//��������� ������� ��� ����� (location = 1)
		glEnableVertexAttribArray(1);//������� �������� 1-�� ������� � �������
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);//���������� ������� �����
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);//��������� ������



		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(pWindow))//�������� ����� ������ �� ���� ������� ����
		{
			/* Render here*/
			glClear(GL_COLOR_BUFFER_BIT);//������� ������ �� �������� ��������

			pDefaultShaderProgram->use();
			//������ ���������
			//glUseProgram(shader_program);//����������� �������

			glBindVertexArray(vao);//���������� vertex_atribut_object, ������� ����� ����������
			glDrawArrays(GL_TRIANGLES, 0, 3);//���������

			/* Swap front and back buffers */
			glfwSwapBuffers(pWindow);//������� ������ ������� �������� � ������ ������ ���������� ����. 

			/* Poll for and process events */
			glfwPollEvents();//��������� ���� �������
		}
	}

	glfwTerminate();//������������ ���� ��������
	return 0;
}