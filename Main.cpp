#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Texture.h"
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
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Lewy dolny r�g
        -0.5f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, // Lewy g�rny r�g
        0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Prawy g�rny r�g
        0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f // Prawy dolny lewy
    };

    GLuint indices[] = {
        0, 2, 1, // G�rny tr�jk�t
        0, 3, 2 // Dolny tr�jk�t
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

    // Po��czenie atrybut�w VBO takich jak koordynaty i kolory do VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Odbindowanie wszystkiego aby zapobiec przypadkowej modyfikacji
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Zdobycie ID uniformu nazwanego "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Tekstura
    Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    popCat.texUnit(shaderProgram, "tex0", 0);

    // G��wna p�tla while
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Powiedzenie OpenGL, kt�rego programu shader u�y�
        shaderProgram.Activate();
        // Przypisanie warto�ci uniformowi; ZAWSZE PO aktywacji shader programu
        glUniform1f(uniID, 0.5f);
        popCat.Bind();
        //Bindowanie VAO aby OpenGL wiedzia� �eby go u�y�
        VAO1.Bind();
        // Rysowanie tr�jk�ta
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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