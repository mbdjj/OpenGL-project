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
    // Za³¹czenie kodu Ÿród³owego vertex shadera do obiektu
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Kompilacja vertex shadera na kod maszynowy
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    // Tworzenie fragment shadera
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Za³¹czenie kodu Ÿród³owego fragment shadera do obiektu
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Kompilacja fragment shadera na kod maszynowy
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "FRAGMENT");

    // Tworzenie programu shadera
    ID = glCreateProgram();
    // Za³¹czenie vertex i fragment shaderów do programu
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Po³¹czenie shaderów w shader programie
    glLinkProgram(ID);
    compileErrors(ID, "PROGRAM");

    // Usuniêcie niepotrzebnych shaderów
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) {
    GLint hasCompiled;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
        }
    }
}