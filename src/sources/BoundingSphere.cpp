#include <BoundingSphere.hpp>

BoundingSphere::BoundingSphere(const GLfloat& radius, const string& name, const Transform& transform):
    radius(radius),
    BoundingVolume(name, transform) {
}

BoundingSphere::BoundingSphere(const BoundingSphere& sphere):
    radius(sphere.radius),
    BoundingVolume(sphere.name, sphere.transform) {
    vertices = sphere.vertices;
    children = sphere.children;
    updateVBO();
}

BoundingSphere::BoundingSphere(BoundingSphere&& sphere) {
    name = move(sphere.name);
    transform = move(sphere.transform);
    children = move(sphere.children);
    VAO = move(sphere.VAO);
    VBO = move(sphere.VBO);
    radius = move(sphere.radius);
    vertices = move(sphere.vertices);
}

BoundingSphere& BoundingSphere::operator=(const BoundingSphere& sphere) noexcept {
    name = sphere.name;
    transform = sphere.transform;
    children = sphere.children;
    radius = sphere.radius;
    vertices = sphere.vertices;

    updateVBO();
    
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

    updateVBO();

    return *this;
}

vector<Vertex> BoundingSphere::getVerticesForGrid(void) const noexcept {
    vector<Vertex> vertices = vector<Vertex>(BoundingSphere::POINTS_PER_RING * 6, {
        vec3(0.f, 0.f, 0.f),
        vec3(1.f, 0.f, 0.f),
        vec3(1.f, 1.f, 1.f),
        vec2()
    });

    constexpr GLfloat ANGLE_SPACING = 2.f * pi<GLfloat>() / static_cast<GLfloat>(BoundingSphere::POINTS_PER_RING);
    constexpr GLint DOUBLE_POINTS_PER_RING = BoundingSphere::POINTS_PER_RING * 2;
    constexpr GLint DOUBLE_POINTS_PER_RING_TIMES_TWO = DOUBLE_POINTS_PER_RING * 2;
    constexpr GLint DOUBLE_POINTS_PER_RING_TIMES_THREE = DOUBLE_POINTS_PER_RING * 3;
    
    // ring centered along the x axis
    mat3x3 R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(1.f, 0.f, 0.f)));
    vertices[0].position = vec3(0.f, 0.f, radius);
    vertices[1].position = R * vertices[0].position;
    for (GLint i = 2; i < DOUBLE_POINTS_PER_RING; i += 2) {
        vertices[i].position = vertices[i - 1].position;
        vertices[i + 1].position = R * vertices[i].position;
    }

    // ring centered along the y axis
    R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(0.f, 1.f, 0.f)));
    vertices[DOUBLE_POINTS_PER_RING].position = vec3(0.f, 0.f, radius);
    vertices[DOUBLE_POINTS_PER_RING].normal = vec3(0.f, 1.f, 0.f);
    vertices[DOUBLE_POINTS_PER_RING + 1].position = R * vertices[0].position;
    vertices[DOUBLE_POINTS_PER_RING + 1].normal = vec3(0.f, 1.f, 0.f);
    for (GLint i = DOUBLE_POINTS_PER_RING + 2; i < DOUBLE_POINTS_PER_RING_TIMES_TWO; i += 2) {
        vertices[i].position = vertices[i - 1].position;
        vertices[i].normal = vertices[i - 1].normal;
        vertices[i + 1].position = R * vertices[i].position;
        vertices[i + 1].normal = R * vertices[i].normal;
    }

    // ring centered along the z axis
    R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(0.f, 0.f, 1.f)));
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].position = vec3(0.f, radius, 0.f);
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].normal = vec3(0.f, 0.f, 1.f);
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO + 1].position = R * vertices[0].position;
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
    vertices = getVerticesForGrid();

    vec4 temp = transform.getMatrix() * vec4(radius, 0.f, 0.f, 0.f);
    this->radius = length(temp);
    
    updateVBO();
}

bool BoundingSphere::intersectsVolume(BoundingVolume*& boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 offset = bSphere->getCenter() - getCenter();
        const float dist2 = dot(offset, offset);
        const float radiusSum = radius + bSphere->getRadius();

        return dist2 <= radiusSum * radiusSum;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->intersectsVolume(self);
}

bool BoundingSphere::enclosesVolume(BoundingVolume*& boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 diff = getCenter() - bSphere->getCenter();
        const float fullDist = length(diff) + bSphere->getRadius();

        return fullDist <= radius;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->isEnclosedByVolume(self);
}

bool BoundingSphere::isEnclosedByVolume(BoundingVolume*& boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 diff = getCenter() - bSphere->getCenter();
        const float fullDist = length(diff) + radius;
        const float& bRadius = bSphere->getRadius();

        return fullDist <= bRadius;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->enclosesVolume(self);
}

void BoundingSphere::update(const Transform& newTransform) {
    SceneObject::update(newTransform);

    // update radius
    vec4 temp = transform.getMatrix() * vec4(radius, 0.f, 0.f, 0.f);
    radius = length(temp);
}