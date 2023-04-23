#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

int main() {
    // Inicjalizacja GLFW
    glfwInit();

    // Informujemy GLFW, ¿e u¿ywamy OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Informujemy GLFW, ¿e u¿ywamy profilu CORE
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Koordynaty wierzcho³ków
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lewy dolny róg
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Prawy dolny róg
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Górny róg
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Œrodkowy lewy
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Œrodkowy prawy
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Œrodkowy dolny
    };

    GLuint indices[] = {
        0, 3, 5, // Lewy dolny trójk¹t
        3, 2, 4, // Prawy dolny trójk¹t
        5, 4, 1 // Górny trójk¹t
    };

    // Tworzymy objekt GLFW 800x800 px i nazywamy go "OpenGL - projekt"
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL - projekt", NULL, NULL);
    // Sprawdzamy czy wyst¹pi³ b³¹d przy tworzeniu okna
    if (window == NULL) {
        std::cout << "Nie mo¿na by³o utworzyæ okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Przedstawiamy okno obecnemu contextowi
    glfwMakeContextCurrent(window);

    // £adujemy GLADa aby skonfigurowa³ OpenGL
    gladLoadGL();

    // Ustalamy viewport naszego okna OpenGL
    glViewport(0, 0, 800, 800);

    // Tworzenie obiektu shader
    Shader shaderProgram("default.vert", "default.frag");

    // Tworzenie Vertex Array Object i bindowanie go
    VAO VAO1;
    VAO1.Bind();

    // Tworzenie Vertex Buffer Object
    VBO VBO1(vertices, sizeof(vertices));
    // Tworzenie Element Buffer Object
    EBO EBO1(indices, sizeof(indices));

    // Po³¹czenie VBO do VAO
    VAO1.LinkVBO(VBO1, 0);
    // Odbindowanie wszystkiego aby zapobiec przypadkowej modyfikacji
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // G³ówna pêtla while
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Powiedzenie OpenGL, którego programu shader u¿yæ
        shaderProgram.Activate();
        //Bindowanie VAO aby OpenGL wiedzia³ ¿eby go u¿yæ
        VAO1.Bind();
        // Rysowanie trójk¹ta
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // Ogarniamy wszystkie eventy GLFW
        glfwPollEvents();
    }

    // Usuniêcie wszystkich stworzonych obiektów
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    // Usuwamy okno przed zakoñczeniem programu
    glfwDestroyWindow(window);
    // Zakañczamy GLFW przed zakoñczeniem programu
    glfwTerminate();
    return 0;
}