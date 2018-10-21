#include <BoundingSphere.hpp>

BoundingSphere::BoundingSphere(const GLfloat& radius, const string& name, const Transform& transform):
    radius(radius),
    BoundingVolume(name, transform) {
    vertices = getVerticesForGrid();
    initialize();
}

BoundingSphere::BoundingSphere(const BoundingSphere& sphere):
    radius(sphere.radius),
    BoundingVolume(sphere.name, sphere.transform) {
    vertices = sphere.vertices;
    children = sphere.children;
    shader = sphere.shader;
    initialize();
}

BoundingSphere::BoundingSphere(BoundingSphere&& sphere) {
    name = move(sphere.name);
    transform = move(sphere.transform);
    children = move(sphere.children);
    VAO = move(sphere.VAO);
    VBO = move(sphere.VBO);
    radius = move(sphere.radius);
    vertices = move(sphere.vertices);
    shader = move(sphere.shader);
}

BoundingSphere& BoundingSphere::operator=(const BoundingSphere& sphere) noexcept {
    name = sphere.name;
    transform = sphere.transform;
    children = sphere.children;
    radius = sphere.radius;
    shader = sphere.shader;
    
    return *this;
}

BoundingSphere& BoundingSphere::operator=(BoundingSphere&& sphere) noexcept {
    deallocate();

    name = move(sphere.name);
    transform = move(sphere.transform);
    children = move(sphere.children);
    VAO = move(sphere.VAO);
    VBO = move(sphere.VBO);
    radius = move(sphere.radius);
    vertices = move(sphere.vertices);
    shader = move(sphere.shader);

    return *this;
}

vector<Vertex> BoundingSphere::getVerticesForGrid(void) const noexcept {
    vector<Vertex> vertices = vector<Vertex>(
        BoundingSphere::POINTS_PER_RING * 6,
        {
            vec3(0.f, 0.f, 0.f),
            vec3(1.f, 0.f, 0.f),
            vec3(1.f, 1.f, 1.f),
            vec2()
        }
    );

    constexpr GLfloat ANGLE_SPACING = 2.f * pi<GLfloat>() / static_cast<GLfloat>(BoundingSphere::POINTS_PER_RING);
    constexpr GLint DOUBLE_POINTS_PER_RING = BoundingSphere::POINTS_PER_RING * 2;
    constexpr GLint DOUBLE_POINTS_PER_RING_TIMES_TWO = DOUBLE_POINTS_PER_RING * 2;
    constexpr GLint DOUBLE_POINTS_PER_RING_TIMES_THREE = DOUBLE_POINTS_PER_RING * 3;
    
    // ring centered along the x axis
    mat3x3 R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(1.f, 0.f, 0.f)));
    vertices[0].position = vec3(0.f, 0.f, 1.f);
    vertices[1].position = R * vertices[0].position;
    for (GLint i = 2; i < DOUBLE_POINTS_PER_RING; i += 2) {
        vertices[i].position = vertices[i - 1].position;
        vertices[i + 1].position = R * vertices[i].position;
    }

    // ring centered along the y axis
    R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(0.f, 1.f, 0.f)));
    vertices[DOUBLE_POINTS_PER_RING].position = vec3(0.f, 0.f, 1.f);
    vertices[DOUBLE_POINTS_PER_RING].normal = vec3(0.f, 1.f, 0.f);
    vertices[DOUBLE_POINTS_PER_RING + 1].position = R * vertices[DOUBLE_POINTS_PER_RING].position;
    vertices[DOUBLE_POINTS_PER_RING + 1].normal = vec3(0.f, 1.f, 0.f);
    for (GLint i = DOUBLE_POINTS_PER_RING + 2; i < DOUBLE_POINTS_PER_RING_TIMES_TWO; i += 2) {
        vertices[i].position = vertices[i - 1].position;
        vertices[i].normal = vertices[i - 1].normal;
        vertices[i + 1].position = R * vertices[i].position;
        vertices[i + 1].normal = R * vertices[i].normal;
    }

    // ring centered along the z axis
    R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(0.f, 0.f, 1.f)));
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].position = vec3(0.f, 1.f, 0.f);
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].normal = vec3(0.f, 0.f, 1.f);
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO + 1].position = R * vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].position;
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO + 1].normal = vec3(0.f, 0.f, 1.f);
    for (GLint i = DOUBLE_POINTS_PER_RING_TIMES_TWO + 2; i < DOUBLE_POINTS_PER_RING_TIMES_THREE; i += 2) {
        vertices[i].position = vertices[i - 1].position;
        vertices[i].normal = vertices[i - 1].normal;
        vertices[i + 1].position = R * vertices[i].position;
        vertices[i + 1].normal = R * vertices[i].normal;
    }

    return vertices;
}

const GLfloat& BoundingSphere::getRadius(void) const noexcept {
    return radius;
}

void BoundingSphere::setRadius(const GLfloat& radius) noexcept {
    this->radius = radius;
}

GLfloat BoundingSphere::getActualRadius(void) const noexcept {
    const vec3 scaledRadii = glm::abs(transform.getScale()) * vec3(radius, radius, radius);
    return max(max(scaledRadii.x, scaledRadii.y), scaledRadii.z);
}

void BoundingSphere::draw(const mat4& ProjectionViewMatrix) const {
    if (shader != nullptr && this->renderVolume) {
        const mat4 model = (Transform(transform.getTranslationAndRotation()).getMatrix());
        const GLfloat radius = getActualRadius();

        shader->use();
        shader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
        shader->setMat4("model", value_ptr(model));
        shader->setFloat("radius", radius);

        glBindVertexArray(getVAO());
        glDrawArrays(GL_LINES, 0, (GLsizei)getVertices().size());
        glBindVertexArray(0);
    }

    SceneObject::draw(ProjectionViewMatrix);
}