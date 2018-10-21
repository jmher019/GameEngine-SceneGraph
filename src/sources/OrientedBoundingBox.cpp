#include <OrientedBoundingBox.hpp>

OrientedBoundingBox::OrientedBoundingBox(const vec3& halfExtents, const string& name, const Transform& transform):
    halfExtents(halfExtents),
    BoundingVolume(name, transform) {
    vertices = getVerticesForGrid();
    initialize();
}

OrientedBoundingBox::OrientedBoundingBox(const OrientedBoundingBox& obb):
    halfExtents(obb.halfExtents),
    BoundingVolume(obb.name, obb.transform) {
    vertices = obb.vertices;
    children = obb.children;
    shader = obb.shader;
    initialize();
}

OrientedBoundingBox::OrientedBoundingBox(OrientedBoundingBox&& obb) {
    name = move(obb.name);
    transform = move(obb.transform);
    children = move(obb.children);
    VAO = move(obb.VAO);
    VBO = move(obb.VBO);
    halfExtents = move(obb.halfExtents);
    vertices = move(obb.vertices);
    shader = move(obb.shader);
}

OrientedBoundingBox& OrientedBoundingBox::operator=(const OrientedBoundingBox& obb) noexcept {
    name = obb.name;
    transform = obb.transform;
    children = obb.children;
    halfExtents = obb.halfExtents;
    shader = obb.shader;

    return *this;
}

OrientedBoundingBox& OrientedBoundingBox::operator=(OrientedBoundingBox&& obb) noexcept {
    deallocate();

    name = move(obb.name);
    transform = move(obb.transform);
    children = move(obb.children);
    VAO = move(obb.VAO);
    VBO = move(obb.VBO);
    halfExtents = move(obb.halfExtents);
    vertices = move(obb.vertices);
    shader = move(obb.shader);

    return *this;
}

vector<Vertex> OrientedBoundingBox::getVerticesForGrid(void) const noexcept {
    vector<Vertex> vertices = vector<Vertex>(24, {
        vec3(0.f, 0.f, 0.f),
        vec3(1.f, 0.f, 0.f),
        vec3(1.f, 1.f, 1.f),
        vec2()
    });

    vertices[0].position = vec3(-1.f, 1.f, 1.f);
    vertices[1].position = vec3(1.f, 1.f, 1.f);
    vertices[2].position = vec3(1.f, 1.f, 1.f);
    vertices[3].position = vec3(1.f, -1.f, 1.f);
    vertices[4].position = vec3(1.f, -1.f, 1.f);
    vertices[5].position = vec3(-1.f, -1.f, 1.f);
    vertices[6].position = vec3(-1.f, -1.f, 1.f);
    vertices[7].position = vec3(-1.f, 1.f, 1.f);

    vertices[8].position = vec3(-1.f, 1.f, -1.f);
    vertices[9].position = vec3(1.f, 1.f, -1.f);
    vertices[10].position = vec3(1.f, 1.f, -1.f);
    vertices[11].position = vec3(1.f, -1.f, -1.f);
    vertices[12].position = vec3(1.f, -1.f, -1.f);
    vertices[13].position = vec3(-1.f, -1.f, -1.f);
    vertices[14].position = vec3(-1.f, -1.f, -1.f);
    vertices[15].position = vec3(-1.f, 1.f, -1.f);

    vertices[16].position = vec3(-1.f, 1.f, 1.f);
    vertices[17].position = vec3(-1.f, 1.f, -1.f);
    vertices[18].position = vec3(1.f, 1.f, 1.f);
    vertices[19].position = vec3(1.f, 1.f, -1.f);
    vertices[20].position = vec3(1.f, -1.f, 1.f);
    vertices[21].position = vec3(1.f, -1.f, -1.f);
    vertices[22].position = vec3(-1.f, -1.f, 1.f);
    vertices[23].position = vec3(-1.f, -1.f, -1.f);

    return vertices;
}

const vec3& OrientedBoundingBox::getHalfExtents(void) const noexcept {
    return halfExtents;
}

void OrientedBoundingBox::setHalfExtents(const vec3& halfExtents) noexcept {
    this->halfExtents = halfExtents;
}

vec3 OrientedBoundingBox::getActualHalfExtents(void) const noexcept {
    return halfExtents * glm::abs(transform.getScale());
}

vec3 OrientedBoundingBox::getXAxis(void) const noexcept {
    const Transform translationAndRotation(transform.getTranslationAndRotation());
    return normalize(vec3(translationAndRotation.getMatrix() * vec4(1.f, 0.f, 0.f, 0.f)));
}

vec3 OrientedBoundingBox::getYAxis(void) const noexcept {
    const Transform translationAndRotation(transform.getTranslationAndRotation());
    return normalize(vec3(translationAndRotation.getMatrix() * vec4(0.f, 1.f, 0.f, 0.f)));
}

vec3 OrientedBoundingBox::getZAxis(void) const noexcept {
    const Transform translationAndRotation(transform.getTranslationAndRotation());
    return normalize(vec3(translationAndRotation.getMatrix() * vec4(0.f, 0.f, 1.f, 0.f)));
}

vector<Triangle> OrientedBoundingBox::getTriangles(void) const noexcept {
    vector<Triangle> triangles;

    const vec3 halfExtents = getActualHalfExtents();
    const vec3 center = getCenter();
    const vec3 transformedX = halfExtents.x * getXAxis();
    const vec3 transformedY = halfExtents.y * getYAxis();
    const vec3 transformedZ = halfExtents.z * getZAxis();

    // +x transformed face
    triangles.push_back(Triangle(
        center + transformedX - transformedY + transformedZ,
        center + transformedX - transformedY - transformedZ,
        center + transformedX + transformedY + transformedZ
    ));

    triangles.push_back(Triangle(
        center + transformedX - transformedY - transformedZ,
        center + transformedX + transformedY - transformedZ,
        center + transformedX + transformedY + transformedZ
    ));

    // -x transformed face
    triangles.push_back(Triangle(
        center - transformedX - transformedY + transformedZ,
        center - transformedX + transformedY + transformedZ,
        center - transformedX - transformedY - transformedZ
    ));

    triangles.push_back(Triangle(
        center - transformedX - transformedY - transformedZ,
        center - transformedX + transformedY + transformedZ,
        center - transformedX + transformedY - transformedZ
    ));

    // +y transformed face
    triangles.push_back(Triangle(
        center - transformedX + transformedY + transformedZ,
        center + transformedX + transformedY + transformedZ,
        center - transformedX + transformedY - transformedZ
    ));

    triangles.push_back(Triangle(
        center + transformedX + transformedY + transformedZ,
        center + transformedX + transformedY - transformedZ,
        center - transformedX + transformedY - transformedZ
    ));

    // -y transformed face
    triangles.push_back(Triangle(
        center - transformedX - transformedY + transformedZ,
        center - transformedX - transformedY - transformedZ,
        center + transformedX - transformedY + transformedZ
    ));

    triangles.push_back(Triangle(
        center + transformedX - transformedY + transformedZ,
        center - transformedX - transformedY - transformedZ,
        center + transformedX - transformedY - transformedZ
    ));

    // +z transformed face
    triangles.push_back(Triangle(
        center - transformedX - transformedY + transformedZ,
        center + transformedX - transformedY + transformedZ,
        center - transformedX + transformedY + transformedZ
    ));

    triangles.push_back(Triangle(
        center + transformedX - transformedY + transformedZ,
        center + transformedX + transformedY + transformedZ,
        center - transformedX + transformedY + transformedZ
    ));

    // -z transformed face
    triangles.push_back(Triangle(
        center - transformedX - transformedY - transformedZ,
        center - transformedX + transformedY - transformedZ,
        center + transformedX - transformedY - transformedZ
    ));

    triangles.push_back(Triangle(
        center + transformedX - transformedY - transformedZ,
        center - transformedX + transformedY - transformedZ,
        center + transformedX + transformedY - transformedZ
    ));

    return triangles;
}

void OrientedBoundingBox::draw(const mat4& ProjectionViewMatrix) const {
    if (shader != nullptr && this->renderVolume) {
        const mat4 model = transform.getMatrix();
        
        shader->use();
        shader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
        shader->setMat4("model", value_ptr(model));
        shader->setVec3("halfExtents", value_ptr(halfExtents));

        glBindVertexArray(getVAO());
        glDrawArrays(GL_LINES, 0, (GLsizei)getVertices().size());
        glBindVertexArray(0);
    }

    SceneObject::draw(ProjectionViewMatrix);
}