#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
	// Inicjalizacja GLFW
	glfwInit();

	// Informujemy GLFW, �e u�ywamy OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Informujemy GLFW, �e u�ywamy profilu CORE
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tworzymy objekt GLFW 800x800 px i nazywamy go "OpenGL - projekt"
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL - projekt", NULL, NULL);
	// Sprawdzamy czy wyst�pi� b��d przy tworzeniu okna
	if (window == NULL) {
		std::cout << "Nie mo�na by�o utworzy� okna GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Przedstawiamy okno obecnemu contextowi
	glfwMakeContextCurrent(window);

	// �adujemy GLADa aby skonfigurowa� OpenGL
	gladLoadGL();

	// Ustalamy viewport naszego okna OpenGL
	glViewport(0, 0, 800, 800);

	// Ustalamy kolor t�a
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Czy�cimy tylny buffer i przypisujemy mu nowy kolor
	glClear(GL_COLOR_BUFFER_BIT);
	// Zamieniamy buffer tylny z przednim
	glfwSwapBuffers(window);

	// G��wna p�tla while
	while (!glfwWindowShouldClose(window)) {
		// Ogarniamy wszystkie eventy GLFW
		glfwPollEvents();
	}

	// Usuwamy okno przed zako�czeniem programu
	glfwDestroyWindow(window);
	// Zaka�czamy GLFW przed zako�czeniem programu
	glfwTerminate();
	return 0;
}