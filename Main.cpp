#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Kod �r�d�owy vertex shadera
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// Kod �r�d�owy fragment shadera
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // G�rny r�g
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

    // Tworzenie vertex shadera
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Za��czenie kodu �r�d�owego vertex shadera do obiektu
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Kompilacja vertex shadera na kod maszynowy
    glCompileShader(vertexShader);

    // Tworzenie fragment shadera
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Za��czenie kodu �r�d�owego fragment shadera do obiektu
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Kompilacja fragment shadera na kod maszynowy
    glCompileShader(fragmentShader);

    // Tworzenie programu shadera
    GLuint shaderProgram = glCreateProgram();
    // Za��czenie vertex i fragment shader�w do programu
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Po��czenie shader�w w shader programie
    glLinkProgram(shaderProgram);

    // Usuni�cie niepotrzebnych shader�w
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
    // Przedstawienie wierzcho�k�w VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Konfiguracja atrybut�w aby OpenGL wiedzia� jak odczyta� VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // W��czenie atrybut�w aby OpenGL wiedzia� �eby ich u�y�
    glEnableVertexAttribArray(0);

    // Bindowanie VBO i VAO do 0 �eby ich przez przypadek nie zmodyfikowa�
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Ustalamy kolor t�a
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Czy�cimy tylny buffer i przypisujemy mu nowy kolor
    glClear(GL_COLOR_BUFFER_BIT);
    // Zamieniamy buffer tylny z przednim
    glfwSwapBuffers(window);

    // G��wna p�tla while
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Powiedzenie OpenGL, kt�rego programu shader u�y�
        glUseProgram(shaderProgram);
        //Bindowanie VAO aby OpenGL wiedzia� �eby go u�y�
        glBindVertexArray(VAO);
        // Rysowanie tr�jk�ta
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        // Ogarniamy wszystkie eventy GLFW
        glfwPollEvents();
    }

    // Usuni�cie wszystkich stworzonych obiekt�w
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Usuwamy okno przed zako�czeniem programu
    glfwDestroyWindow(window);
    // Zaka�czamy GLFW przed zako�czeniem programu
    glfwTerminate();
    return 0;
}