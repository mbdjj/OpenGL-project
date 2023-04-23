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

    // Informujemy GLFW, �e u�ywamy OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Informujemy GLFW, �e u�ywamy profilu CORE
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Koordynaty wierzcho�k�w
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lewy dolny r�g
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Prawy dolny r�g
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // G�rny r�g
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // �rodkowy lewy
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // �rodkowy prawy
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // �rodkowy dolny
    };

    GLuint indices[] = {
        0, 3, 5, // Lewy dolny tr�jk�t
        3, 2, 4, // Prawy dolny tr�jk�t
        5, 4, 1 // G�rny tr�jk�t
    };

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

    // Tworzenie obiektu shader
    Shader shaderProgram("default.vert", "default.frag");

    // Tworzenie Vertex Array Object i bindowanie go
    VAO VAO1;
    VAO1.Bind();

    // Tworzenie Vertex Buffer Object
    VBO VBO1(vertices, sizeof(vertices));
    // Tworzenie Element Buffer Object
    EBO EBO1(indices, sizeof(indices));

    // Po��czenie VBO do VAO
    VAO1.LinkVBO(VBO1, 0);
    // Odbindowanie wszystkiego aby zapobiec przypadkowej modyfikacji
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // G��wna p�tla while
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Powiedzenie OpenGL, kt�rego programu shader u�y�
        shaderProgram.Activate();
        //Bindowanie VAO aby OpenGL wiedzia� �eby go u�y�
        VAO1.Bind();
        // Rysowanie tr�jk�ta
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // Ogarniamy wszystkie eventy GLFW
        glfwPollEvents();
    }

    // Usuni�cie wszystkich stworzonych obiekt�w
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    // Usuwamy okno przed zako�czeniem programu
    glfwDestroyWindow(window);
    // Zaka�czamy GLFW przed zako�czeniem programu
    glfwTerminate();
    return 0;
}