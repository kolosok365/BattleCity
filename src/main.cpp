#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

int main(void)
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

   if(!gladLoadGL()){ //инициализация библиотеки GLAD
      std::cout<<"Can't load GLAD!"<<std::endl;
      return -1;
   }

   std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
   std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
   
   glClearColor(1,1,0,1);

   /* Loop until the user closes the window */
   while (!glfwWindowShouldClose(pWindow))//проверка флага должно ли быть закрыто окно
   {
      /* Render here*/
      glClear(GL_COLOR_BUFFER_BIT);//очищает буферы до заданных значений

      /* Swap front and back buffers */
      glfwSwapBuffers(pWindow);//функция меняет местами передний и задний буферы указанного окна. 

      /* Poll for and process events */
      glfwPollEvents();//обработка всех событий
   }

   glfwTerminate();//освобождение всех ресурсов
   return 0;
}