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

    // Informujemy GLFW, ¿e u¿ywamy OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Informujemy GLFW, ¿e u¿ywamy profilu CORE
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Koordynaty wierzcho³ków
    GLfloat vertices[] = {
        //   koordynaty      /       kolory       /  koordynaty textury  /     normals
        -0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,     0.0f, -1.0f,  0.0f, // Dolna strona
        -0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,     0.0f, -1.0f,  0.0f,
         0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,     0.0f, -1.0f,  0.0f,
         0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,     0.0f, -1.0f,  0.0f,

        -0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,    -0.8f,  0.5f,  0.0f, // Lewa strona
        -0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,    -0.8f,  0.5f,  0.0f,
         0.0f,  0.8f,  0.0f,    0.92f, 0.86f, 0.76f,    2.5f, 5.0f,    -0.8f,  0.5f,  0.0f,

        -0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,     0.0f,  0.5f, -0.8f, // Tylna strona
         0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,     0.0f,  0.5f, -0.8f,
         0.0f,  0.8f,  0.0f,    0.92f, 0.86f, 0.76f,    2.5f, 5.0f,     0.0f,  0.5f, -0.8f,

         0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,     0.8f,  0.5f,  0.0f, // Prawa strona
         0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,     0.8f,  0.5f,  0.0f,
         0.0f,  0.8f,  0.0f,    0.92f, 0.86f, 0.76f,    2.5f, 5.0f,     0.8f,  0.5f,  0.0f,

         0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    5.0f, 0.0f,     0.0f,  0.5f,  0.8f, // Przednia strona
        -0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,     0.0f,  0.5f,  0.8f,
         0.0f,  0.8f,  0.0f,    0.92f, 0.86f, 0.76f,    2.5f, 5.0f,     0.0f,  0.5f,  0.8f
    };

    // Kolejnoœæ wierzcho³ków
    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3,
        4, 6, 5,
        7, 9, 8,
        10, 12, 11,
        13, 15, 14,
    };

    // Wierzcho³ki œwiec¹cej kostki
    GLfloat lightVertices[] = {
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };

    // Kolejnoœæ wierzcho³ków kostki
    GLuint lightIndices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
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
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    // Odbindowanie wszystkiego aby zapobiec przypadkowej modyfikacji
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Shader dla œwiec¹cej kostki
    Shader lightShader("light.vert", "light.frag");
    // Zbindowanie VAO
    VAO lightVAO;
    lightVAO.Bind();

    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));
    // Po³¹czenie atrybutów z VAO
    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    // Odbindowanie, ¿eby zapobiec przypadkowej zmianie
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);

    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Tekstura
    Texture popCat("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    popCat.texUnit(shaderProgram, "tex0", 0);

    // W³¹czenie buffera g³êbokoœci
    glEnable(GL_DEPTH_TEST);

    // Tworzenie obiektu kamery
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    // G³ówna pêtla while
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sterowanie kamer¹ za pomoc¹ klawiatury i myszy
        camera.Inputs(window);
        // Uaktualnienie matrycy kamery i wyeksportowanie jej do vertex shadera
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Powiedzenie OpenGL, którego programu shader u¿yæ
        shaderProgram.Activate();
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        camera.Matrix(shaderProgram, "camMatrix");

        // Binodowanie tekstury, aby pokaza³a siê przy renederowaniu
        popCat.Bind();
        // Bindowanie VAO aby OpenGL wiedzia³ ¿eby go u¿yæ
        VAO1.Bind();
        // Rysowanie trójk¹tów
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);


        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix");
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

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
    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    lightShader.Delete();

    // Usuwamy okno przed zakoñczeniem programu
    glfwDestroyWindow(window);
    // Zakañczamy GLFW przed zakoñczeniem programu
    glfwTerminate();
    return 0;
}