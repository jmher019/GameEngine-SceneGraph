#include <Mesh.hpp>

Mesh::Mesh(const vector<Vertex>& vertices, const string& name, const Transform& transform):
    SceneObject(name, transform),
    vertices(vertices) {
    initialize();
}

Mesh::Mesh(vector<Vertex>&& vertices, const string& name, const Transform& transform) :
    SceneObject(name, transform),
    vertices(std::forward<vector<Vertex>>(vertices)) {
    initialize();
}

Mesh::Mesh(Mesh&& mesh):
    SceneObject(std::move(mesh.name), std::move(mesh.transform)),
    VBO(std::move(mesh.VBO)),
    VAO(std::move(mesh.VAO)),
    vertices(std::move(mesh.vertices)){
}

Mesh::~Mesh(void) {
    deallocate();
}

void Mesh::initialize(void) noexcept {
    if (!vertices.empty()) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

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
    }
}

void Mesh::deallocate(void) noexcept {
    if (!vertices.empty()) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        vertices.clear();
    }
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    deallocate();

    name = std::move(other.name);
    transform = std::move(other.transform);
    VBO = std::move(other.VBO);
    VAO = std::move(other.VAO);
    vertices = std::move(other.vertices);

    return *this;
}

void Mesh::draw(const mat4& ProjectionViewMatrix) const {
    if (shader != nullptr) {
        const mat4 model = transform.getMatrix();

        shader->use();
        shader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
        shader->setMat4("model", value_ptr(model));
        shader->setMat4("transposeInvertedModel", value_ptr(transpose(inverse(model))));

        LightFactory::loadLightsIntoShader(shader);
        CameraFactory::loadPlayerCameraIntoShader(shader);

        glBindVertexArray(getVAO());
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)getVertices().size());
        glBindVertexArray(0);
    }

    SceneObject::draw(ProjectionViewMatrix);
}

void Mesh::draw(const mat4& ProjectionViewMatrix, const mat4& model) const {
    if (shader != nullptr) {
        shader->use();
        shader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
        shader->setMat4("model", value_ptr(model));
        shader->setMat4("transposeInvertedModel", value_ptr(transpose(inverse(model))));

        LightFactory::loadLightsIntoShader(shader);
        CameraFactory::loadPlayerCameraIntoShader(shader);

        glBindVertexArray(getVAO());
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)getVertices().size());
        glBindVertexArray(0);
    }
}

const vector<Vertex>& Mesh::getVertices(void) const noexcept {
    return vertices;
}

const GLuint& Mesh::getVBO(void) const noexcept {
    return VBO;
}

const GLuint& Mesh::getVAO(void) const noexcept {
    return VAO;
}

const shared_ptr<Shader>& Mesh::getShader(void) const noexcept {
    return shader;
}

void Mesh::setShader(const shared_ptr<Shader>& shader) noexcept {
    this->shader = shader;
}

ostream& operator<< (ostream& out, const Mesh& mesh) {
    out << "Mesh name: " << mesh.getName() << endl;
    out << "Mesh transform:\n" << mesh.getTransform() << endl;
    out << "Mesh VBO: " << mesh.getVBO() << endl;
    out << "Mesh VAO: " << mesh.getVAO() << endl;

    if (!mesh.getChildren().empty()) {
        out << "Mesh children:\n" << endl;
        for (auto& child : mesh.getChildren()) {
            out << *child << endl;
        }
    }

    return out;
}