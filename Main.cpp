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

const unsigned int width = 800;
const unsigned int height = 800;

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

    // Po³¹czenie atrybutów VBO takich jak koordynaty i kolory do VAO
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
    Texture popCat("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    popCat.texUnit(shaderProgram, "tex0", 0);

    // Zmienne pomagaj¹ce przy obracaniu piramidy
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    // W³¹czenie buffera g³êbokoœci
    glEnable(GL_DEPTH_TEST);

    // G³ówna pêtla while
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Powiedzenie OpenGL, którego programu shader u¿yæ
        shaderProgram.Activate();

        // Prosty timer
        double currentTime = glfwGetTime();
        if (currentTime - prevTime >= 1 / 60) {
            rotation += 0.5f;
            prevTime = currentTime;
        }

        // Inicjalizacja matryc, aby nie by³y puste
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        // Nadanie ró¿nych transformacji matrycom
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 1.0, 0.0));
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

        // Wyprowadzenie matryc do vertex shadera
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Przypisanie wartoœci uniformowi; ZAWSZE PO aktywacji shader programu
        glUniform1f(uniID, 0.5f);
        popCat.Bind();
        //Bindowanie VAO aby OpenGL wiedzia³ ¿eby go u¿yæ
        VAO1.Bind();
        // Rysowanie trójk¹tów
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // Ogarniamy wszystkie eventy GLFW
        glfwPollEvents();
    }

    // Usuniêcie wszystkich stworzonych obiektów
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    popCat.Delete();
    shaderProgram.Delete();

    // Usuwamy okno przed zakoñczeniem programu
    glfwDestroyWindow(window);
    // Zakañczamy GLFW przed zakoñczeniem programu
    glfwTerminate();
    return 0;
}