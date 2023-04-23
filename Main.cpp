#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Kod Ÿród³owy vertex shadera
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// Kod Ÿród³owy fragment shadera
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Górny róg
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

    // Tworzenie vertex shadera
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Za³¹czenie kodu Ÿród³owego vertex shadera do obiektu
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Kompilacja vertex shadera na kod maszynowy
    glCompileShader(vertexShader);

    // Tworzenie fragment shadera
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Za³¹czenie kodu Ÿród³owego fragment shadera do obiektu
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Kompilacja fragment shadera na kod maszynowy
    glCompileShader(fragmentShader);

    // Tworzenie programu shadera
    GLuint shaderProgram = glCreateProgram();
    // Za³¹czenie vertex i fragment shaderów do programu
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Po³¹czenie shaderów w shader programie
    glLinkProgram(shaderProgram);

    // Usuniêcie niepotrzebnych shaderów
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Tworzenie Vertex Array Object i Vertex Buffer Object
    GLuint VAO, VBO;

    // Generowanie VAO i VBO z 1 obiektem
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bindowanie VEO jako obecny Vertex Array Object
    glBindVertexArray(VAO);

    // Bindowanie VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Przedstawienie wierzcho³ków VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Konfiguracja atrybutów aby OpenGL wiedzia³ jak odczytaæ VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // W³¹czenie atrybutów aby OpenGL wiedzia³ ¿eby ich u¿yæ
    glEnableVertexAttribArray(0);

    // Bindowanie VBO i VAO do 0 ¿eby ich przez przypadek nie zmodyfikowaæ
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Ustalamy kolor t³a
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Czyœcimy tylny buffer i przypisujemy mu nowy kolor
    glClear(GL_COLOR_BUFFER_BIT);
    // Zamieniamy buffer tylny z przednim
    glfwSwapBuffers(window);

    // G³ówna pêtla while
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Powiedzenie OpenGL, którego programu shader u¿yæ
        glUseProgram(shaderProgram);
        //Bindowanie VAO aby OpenGL wiedzia³ ¿eby go u¿yæ
        glBindVertexArray(VAO);
        // Rysowanie trójk¹ta
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        // Ogarniamy wszystkie eventy GLFW
        glfwPollEvents();
    }

    // Usuniêcie wszystkich stworzonych obiektów
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Usuwamy okno przed zakoñczeniem programu
    glfwDestroyWindow(window);
    // Zakañczamy GLFW przed zakoñczeniem programu
    glfwTerminate();
    return 0;
}