#ifndef BOUNDING_VOLUME_HPP
#define BOUNDING_VOLUME_HPP

#include <SceneObject.hpp>
#include <Shader.hpp>

#include <glm\gtc\type_ptr.hpp>

class BoundingVolume : public SceneObject {
protected:
    GLuint VBO = 0;
    GLuint VAO = 0;
    shared_ptr<Shader> shader = nullptr;
    vector<Vertex> vertices;
    GLboolean renderVolume = false;
    
    BoundingVolume(const string& name = string(""), const Transform& transform = Transform()):
        SceneObject(name, transform) {        
    };

    virtual void initialize(void) noexcept {
        if (!VAO) {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
        }

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex colors
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        // vertex texture coords
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glBindVertexArray(0);
    };

    virtual void deallocate(void) noexcept {
        if (VAO) {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            VAO = 0;
            VBO = 0;
        }
    };

    void updateVBO(void) {
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    };

    virtual vector<Vertex> getVerticesForGrid(void) const noexcept = 0;

public:
    virtual ~BoundingVolume(void) {
        deallocate();
    }

    virtual bool intersectsVolume(BoundingVolume*& boundingVolume) const noexcept = 0;

    virtual bool enclosesVolume(BoundingVolume*& boundingVolume) const noexcept = 0;

    virtual bool isEnclosedByVolume(BoundingVolume*& boundingVolume) const noexcept = 0;

    void draw(const mat4& ProjectionViewMatrix) const override {
        if (shader != nullptr) {
            const mat4 model = transform.getMatrix();

            shader->use();
            shader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
            shader->setMat4("model", value_ptr(model));

            glBindVertexArray(getVAO());
            glDrawArrays(GL_LINES, 0, (GLsizei)getVertices().size());
            glBindVertexArray(0);
        }

        SceneObject::draw(ProjectionViewMatrix);
    };

    const GLuint& getVBO(void) const noexcept {
        return VBO;
    };

    const GLuint& getVAO(void) const noexcept {
        return VAO;
    };

    const vector<Vertex>& getVertices(void) const noexcept {
        return vertices;
    };

    vec3 getCenter(void) const noexcept {
        return vec3(transform.getMatrix()[3]);
    };

    const shared_ptr<Shader>& getShader(void) const noexcept {
        return shader;
    };

    void setShader(const shared_ptr<Shader>& shader) noexcept {
        this->shader = shader;
    };

    void enableRenderVolume(const GLboolean& enable) noexcept {
        this->renderVolume = enable;
    }
};

#endif // !BOUNDING_VOLUME_HPP
