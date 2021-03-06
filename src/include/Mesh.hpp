#ifndef MESH_HPP
#define MESH_HPP

#include <SceneObject.hpp>
#include <Shader.hpp>

#include <glm\gtc\type_ptr.hpp>

class Mesh : public SceneObject {
private:
    GLuint VBO = 0;
    GLuint VAO = 0;
    shared_ptr<Shader> shader = nullptr;
    vector<Vertex> vertices;

    void initialize(void) noexcept;

    void deallocate(void) noexcept;

public:
    Mesh(vector<Vertex>&& vertices, const string& name = string(""), const Transform& transform = Transform());

    Mesh(const Mesh& mesh) = delete;
    
    Mesh(Mesh&& mesh);

    ~Mesh(void);

    Mesh& operator=(const Mesh& other) noexcept = delete;

    Mesh& operator=(Mesh&& other) noexcept;

    void draw(const mat4& ProjectionViewMatrix) const override;

    const vector<Vertex>& getVertices(void) const noexcept;
    
    const GLuint& getVBO(void) const noexcept;
    
    const GLuint& getVAO(void) const noexcept;

    const shared_ptr<Shader>& getShader(void) const noexcept;

    void setShader(const shared_ptr<Shader>& shader) noexcept;
};

ostream& operator<< (ostream& out, const Mesh& mesh);

#endif // !MESH_HPP
