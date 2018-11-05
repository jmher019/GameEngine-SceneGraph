#include <Shader.hpp>

Shader::Shader(const string& vertexPath, const string& fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        throw exception("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDetachShader(id, vertex);
    glDetachShader(id, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const Shader& shader) :
    id(shader.id) {}

Shader::Shader(Shader&& shader) :
    id(shader.id) {}

Shader::~Shader(void) {
    glUseProgram(0);

    GLint flaggedForDeletion;
    glGetProgramiv(id, GL_DELETE_STATUS, &flaggedForDeletion);

    if (flaggedForDeletion == GL_FALSE && glIsProgram(id) == GL_TRUE) {
        glDeleteProgram(id);
    }

    id = 0;
}

void Shader::checkCompileErrors(const GLuint& shader, const string& type) const {
    int success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw exception(string("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ").c_str());
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            throw exception(string("ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ").c_str());
        }
    }
}

void Shader::use(void) const {
    glUseProgram(id);
}

const GLuint& Shader::getId(void) const {
    return id;
}

const GLuint Shader::getUniformBlockIdx(const std::string& uniformBlockName) const {
    return glGetUniformBlockIndex(id, uniformBlockName.c_str());
}

void Shader::setUInt(const std::string& name, const GLuint value) const {
    glUniform1ui(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, const GLfloat value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const GLfloat* vec) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, vec);
}

void Shader::setVec4(const std::string& name, const GLfloat* vec) const {
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, vec);
}

void Shader::setMat4(const std::string& name, const GLfloat* mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, false, mat);
}

ostream& operator<< (ostream& out, const Shader& shader) {
    out << "Shader id: " << shader.getId() << endl;

    return out;
}