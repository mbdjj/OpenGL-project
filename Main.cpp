#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

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
        //   koordynaty      /       kolory          /  koordynaty textury
        -0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,
        -0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,
         0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,
         0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,
         0.0f,  0.8f,  0.0f,    0.92f, 0.86f, 0.76f,    2.5f, 5.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4,
    };

    // Tworzymy objekt GLFW 800x800 px i nazywamy go "OpenGL - projekt"
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL - projekt", NULL, NULL);
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
    glViewport(0, 0, width, height);

    // Tworzenie obiektu shader
    Shader shaderProgram("default.vert", "default.frag");

    // Tworzenie Vertex Array Object i bindowanie go
    VAO VAO1;
    VAO1.Bind();

    // Tworzenie Vertex Buffer Object
    VBO VBO1(vertices, sizeof(vertices));
    // Tworzenie Element Buffer Object
    EBO EBO1(indices, sizeof(indices));

    // Po��czenie atrybut�w VBO takich jak koordynaty i kolory do VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Odbindowanie wszystkiego aby zapobiec przypadkowej modyfikacji
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Tekstura
    Texture popCat("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    popCat.texUnit(shaderProgram, "tex0", 0);

    // W��czenie buffera g��boko�ci
    glEnable(GL_DEPTH_TEST);

    // Tworzenie obiektu kamery
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    // G��wna p�tla while
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Powiedzenie OpenGL, kt�rego programu shader u�y�
        shaderProgram.Activate();

        // Sterowanie kamer� za pomoc� klawiatury i myszy
        camera.Inputs(window);
        // Uaktualnienie matrycy kamery i wyeksportowanie jej do vertex shadera
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        // Binodowanie tekstury, aby pokaza�a si� przy renederowaniu
        popCat.Bind();
        // Bindowanie VAO aby OpenGL wiedzia� �eby go u�y�
        VAO1.Bind();
        // Rysowanie tr�jk�t�w
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // Ogarniamy wszystkie eventy GLFW
        glfwPollEvents();
    }

    // Usuni�cie wszystkich stworzonych obiekt�w
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    popCat.Delete();
    shaderProgram.Delete();

    // Usuwamy okno przed zako�czeniem programu
    glfwDestroyWindow(window);
    // Zaka�czamy GLFW przed zako�czeniem programu
    glfwTerminate();
    return 0;
}