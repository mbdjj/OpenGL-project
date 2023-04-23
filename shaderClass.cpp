#include "shaderClass.h"

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw errno;
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {

	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

    // Tworzenie vertex shadera
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Za��czenie kodu �r�d�owego vertex shadera do obiektu
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Kompilacja vertex shadera na kod maszynowy
    glCompileShader(vertexShader);

    // Tworzenie fragment shadera
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Za��czenie kodu �r�d�owego fragment shadera do obiektu
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Kompilacja fragment shadera na kod maszynowy
    glCompileShader(fragmentShader);

    // Tworzenie programu shadera
    ID = glCreateProgram();
    // Za��czenie vertex i fragment shader�w do programu
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Po��czenie shader�w w shader programie
    glLinkProgram(ID);

    // Usuni�cie niepotrzebnych shader�w
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}