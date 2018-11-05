#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <sstream>

#include <Vertex.hpp>

using namespace std;

class Shader {
private:
    GLuint id;

    void checkCompileErrors(const GLuint& shader, const string& type) const;

public:
    Shader(const string& vertexPath, const string& fragmentPath);

    Shader(const Shader& shader);
    
    Shader(Shader&& shader);
    
    ~Shader(void);

    void use(void) const;
    
    const GLuint& getId(void) const;
    
    const GLuint getUniformBlockIdx(const std::string& uniformBlockName) const;

    void setUInt(const std::string& name, const GLuint value) const;

    void setFloat(const std::string& name, const GLfloat value) const;

    void setVec3(const std::string& name, const GLfloat* vec) const;

    void setVec4(const std::string& name, const GLfloat* vec) const;

    void setMat4(const std::string& name, const GLfloat* mat) const;
};

ostream& operator<< (ostream& out, const Shader& shader);

#endif // !SHADER_HPP
