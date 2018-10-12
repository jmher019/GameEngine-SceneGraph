#include <AxisAlignedBoundingBox.hpp>

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const vec3& initialHalfExtents, const string& name, const Transform& transform):
    initialHalfExtents(initialHalfExtents),
    halfExtents(halfExtents),
    BoundingVolume(name, transform) {
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const AxisAlignedBoundingBox& aabb):
    initialHalfExtents(aabb.initialHalfExtents),
    halfExtents(aabb.halfExtents),
    BoundingVolume(aabb.name, aabb.transform) {
    vertices = aabb.vertices;
    children = aabb.children;
    updateVBO();
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(AxisAlignedBoundingBox&& aabb) {
    name = move(aabb.name);
    transform = move(aabb.transform);
    children = move(aabb.children);
    VAO = move(aabb.VAO);
    VBO = move(aabb.VBO);
    initialHalfExtents = move(aabb.initialHalfExtents);
    halfExtents = move(aabb.halfExtents);
    vertices = move(aabb.vertices);
}

AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator=(const AxisAlignedBoundingBox& aabb) noexcept {
    name = aabb.name;
    transform = aabb.transform;
    children = aabb.children;
    initialHalfExtents = aabb.initialHalfExtents;
    halfExtents = aabb.halfExtents;
    vertices = aabb.vertices;

    updateVBO();

    return *this;
}

AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator=(AxisAlignedBoundingBox&& aabb) noexcept {
    deallocate();

    name = move(aabb.name);
    transform = move(aabb.transform);
    children = move(aabb.children);
    VAO = move(aabb.VAO);
    VBO = move(aabb.VBO);
    initialHalfExtents = move(aabb.initialHalfExtents);
    halfExtents = move(aabb.halfExtents);
    vertices = move(aabb.vertices);

    updateVBO();

    return *this;
}

vector<Vertex> AxisAlignedBoundingBox::getVerticesForGrid(void) const noexcept {
    vector<Vertex> vertices = vector<Vertex>(24, {
        vec3(0.f, 0.f, 0.f),
        vec3(1.f, 0.f, 0.f),
        vec3(1.f, 1.f, 1.f),
        vec2()
    });

    vertices[0].position = vec3(-initialHalfExtents.x, initialHalfExtents.y, initialHalfExtents.z);
    vertices[1].position = vec3(initialHalfExtents.x, initialHalfExtents.y, initialHalfExtents.z);
    vertices[2].position = vec3(initialHalfExtents.x, initialHalfExtents.y, initialHalfExtents.z);
    vertices[3].position = vec3(initialHalfExtents.x, -initialHalfExtents.y, initialHalfExtents.z);
    vertices[4].position = vec3(initialHalfExtents.x, -initialHalfExtents.y, initialHalfExtents.z);
    vertices[5].position = vec3(-initialHalfExtents.x, -initialHalfExtents.y, initialHalfExtents.z);
    vertices[6].position = vec3(-initialHalfExtents.x, -initialHalfExtents.y, initialHalfExtents.z);
    vertices[7].position = vec3(-initialHalfExtents.x, initialHalfExtents.y, initialHalfExtents.z);

    vertices[8].position = vec3(-initialHalfExtents.x, initialHalfExtents.y, -initialHalfExtents.z);
    vertices[9].position = vec3(initialHalfExtents.x, initialHalfExtents.y, -initialHalfExtents.z);
    vertices[10].position = vec3(initialHalfExtents.x, initialHalfExtents.y, -initialHalfExtents.z);
    vertices[11].position = vec3(initialHalfExtents.x, -initialHalfExtents.y, -initialHalfExtents.z);
    vertices[12].position = vec3(initialHalfExtents.x, -initialHalfExtents.y, -initialHalfExtents.z);
    vertices[13].position = vec3(-initialHalfExtents.x, -initialHalfExtents.y, -initialHalfExtents.z);
    vertices[14].position = vec3(-initialHalfExtents.x, -initialHalfExtents.y, -initialHalfExtents.z);
    vertices[15].position = vec3(-initialHalfExtents.x, initialHalfExtents.y, -initialHalfExtents.z);

    vertices[16].position = vec3(-initialHalfExtents.x, initialHalfExtents.y, initialHalfExtents.z);
    vertices[17].position = vec3(-initialHalfExtents.x, initialHalfExtents.y, -initialHalfExtents.z);
    vertices[18].position = vec3(initialHalfExtents.x, initialHalfExtents.y, initialHalfExtents.z);
    vertices[19].position = vec3(initialHalfExtents.x, initialHalfExtents.y, -initialHalfExtents.z);
    vertices[20].position = vec3(initialHalfExtents.x, -initialHalfExtents.y, initialHalfExtents.z);
    vertices[21].position = vec3(initialHalfExtents.x, -initialHalfExtents.y, -initialHalfExtents.z);
    vertices[22].position = vec3(-initialHalfExtents.x, -initialHalfExtents.y, initialHalfExtents.z);
    vertices[23].position = vec3(-initialHalfExtents.x, -initialHalfExtents.y, -initialHalfExtents.z);

    return vertices;
}

const vec3& AxisAlignedBoundingBox::getHalfExtents(void) const noexcept {
    return halfExtents;
}

void AxisAlignedBoundingBox::setHalfExtents(const vec3& halfExtents) noexcept {
    initialHalfExtents = halfExtents;
    vertices = this->getVerticesForGrid();

    const mat3& upperLeft = mat3(transform.getMatrix());
    for (int i = 0; i < 3; i++) {
        this->halfExtents[i] = 0.f;
        for (int j = 0; j < 3; j++) {
            this->halfExtents[i] += fabs(upperLeft[j][i] * initialHalfExtents[j]);
        }
    }

    updateVBO();
}

GLfloat AxisAlignedBoundingBox::getSquaredDistancePtPointAABB(const vec3& pt) const noexcept {
    float sqDist = 0.f;

    const vec3 min = vec3(getCenter()) - halfExtents;
    const vec3 max = vec3(getCenter()) + halfExtents;

    for (int i = 0; i < 3; i++) {
        // For each axis count any excess distance outside box extents
        const float& v = pt[i];
        if (v < min[i]) {
            sqDist += (min[i] - v) * (min[i] - v);
        }
        if (v > max[i]) {
            sqDist += (v - max[i]) * (v - max[i]);
        }
    }
    return sqDist;
}

vector<Triangle> AxisAlignedBoundingBox::getTriangles(void) const {
    vector<Triangle> triangles;

    const vec3 center = vec3(getCenter());

    // +x face
    // center + <he.x, +/-he.y, +/-he.z>
    triangles.push_back(Triangle(
        center + vec3(halfExtents.x, -halfExtents.y, halfExtents.z),
        center + vec3(halfExtents.x, -halfExtents.y, -halfExtents.z),
        center + vec3(halfExtents.x, halfExtents.y, halfExtents.z)
    ));

    triangles.push_back(Triangle(
        center + vec3(halfExtents.x, -halfExtents.y, -halfExtents.z),
        center + vec3(halfExtents.x, halfExtents.y, -halfExtents.z),
        center + vec3(halfExtents.x, halfExtents.y, halfExtents.z)
    ));

    // -x face
    // center + <-he.x, +/-he.y, +/-he.z>
    triangles.push_back(Triangle(
        center + vec3(-halfExtents.x, -halfExtents.y, halfExtents.z),
        center + vec3(-halfExtents.x, halfExtents.y, halfExtents.z),
        center + vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z)
    ));

    triangles.push_back(Triangle(
        center + vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z),
        center + vec3(-halfExtents.x, halfExtents.y, halfExtents.z),
        center + vec3(-halfExtents.x, halfExtents.y, -halfExtents.z)
    ));

    // +y face
    // center + <+/-he.x, he.y, +/-he.z>
    triangles.push_back(Triangle(
        center + vec3(-halfExtents.x, halfExtents.y, halfExtents.z),
        center + vec3(halfExtents.x, halfExtents.y, halfExtents.z),
        center + vec3(-halfExtents.x, halfExtents.y, -halfExtents.z)
    ));

    triangles.push_back(Triangle(
        center + vec3(halfExtents.x, halfExtents.y, halfExtents.z),
        center + vec3(halfExtents.x, halfExtents.y, -halfExtents.z),
        center + vec3(-halfExtents.x, halfExtents.y, -halfExtents.z)
    ));

    // -y face
    // center + <+/-he.x, -he.y, +/-he.z>
    triangles.push_back(Triangle(
        center + vec3(-halfExtents.x, -halfExtents.y, halfExtents.z),
        center + vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z),
        center + vec3(halfExtents.x, -halfExtents.y, halfExtents.z)
    ));

    triangles.push_back(Triangle(
        center + vec3(halfExtents.x, -halfExtents.y, halfExtents.z),
        center + vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z),
        center + vec3(halfExtents.x, -halfExtents.y, -halfExtents.z)
    ));

    // +z face
    // center + <+/-he.x, +/-he.y, he.z>
    triangles.push_back(Triangle(
        center + vec3(-halfExtents.x, -halfExtents.y, halfExtents.z),
        center + vec3(halfExtents.x, -halfExtents.y, halfExtents.z),
        center + vec3(-halfExtents.x, halfExtents.y, halfExtents.z)
    ));

    triangles.push_back(Triangle(
        center + vec3(halfExtents.x, -halfExtents.y, halfExtents.z),
        center + vec3(halfExtents.x, halfExtents.y, halfExtents.z),
        center + vec3(-halfExtents.x, halfExtents.y, halfExtents.z)
    ));

    // -z face
    // center + <+/-he.x, +/-he.y, -he.z>
    triangles.push_back(Triangle(
        center + vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z),
        center + vec3(-halfExtents.x, halfExtents.y, -halfExtents.z),
        center + vec3(halfExtents.x, -halfExtents.y, -halfExtents.z)
    ));

    triangles.push_back(Triangle(
        center + vec3(halfExtents.x, -halfExtents.y, -halfExtents.z),
        center + vec3(-halfExtents.x, halfExtents.y, -halfExtents.z),
        center + vec3(halfExtents.x, halfExtents.y, -halfExtents.z)
    ));

    return triangles;
}

bool AxisAlignedBoundingBox::intersectsVolume(BoundingVolume*& boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 sphereCenter = bSphere->getCenter();
        const float& radius = bSphere->getRadius();
        const float sqDist = getSquaredDistancePtPointAABB(sphereCenter);
        return sqDist <= radius * radius;
    }
    // handle axis aligned bounding box here
    else if (const AxisAlignedBoundingBox* bAABB = dynamic_cast<AxisAlignedBoundingBox*>(boundingVolume)) {
        const vec3& center = getCenter();
        const vec3& bCenter = bAABB->getCenter();
        const vec3& bHalfExents = bAABB->getHalfExtents();

        for (int i = 0; i < 3; i++) {
            if (fabs(center[i] - bCenter[i]) > (halfExtents[i] + bHalfExents[i])) {
                return false;
            }
        }

        return true;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->intersectsVolume(self);
}

bool AxisAlignedBoundingBox::enclosesVolume(BoundingVolume*& boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const float& bRadius = bSphere->getRadius();
        const vec3 bCenter = bSphere->getCenter();
        const vec3 bMin = bCenter - vec3(bRadius, bRadius, bRadius);
        const vec3 bMax = bCenter + vec3(bRadius, bRadius, bRadius);

        const vec3 center = getCenter();
        const vec3 min = center - halfExtents;
        const vec3 max = center + halfExtents;

        for (int i = 0; i < 3; i++) {
            if (bMin[i] < min[i]) return false;
            else if (bMax[i] > max[i]) return false;
        }
        return true;
    }
    // handle axis aligned bounding box here
    else if (const AxisAlignedBoundingBox* bAABB = dynamic_cast<AxisAlignedBoundingBox*>(boundingVolume)) {
        const vec3 bCenter = bAABB->getCenter();
        const vec3 bMin = bCenter - bAABB->getHalfExtents();
        const vec3 bMax = bCenter + bAABB->getHalfExtents();

        const vec3 center = getCenter();
        const vec3 min = center - halfExtents;
        const vec3 max = center + halfExtents;

        for (int i = 0; i < 3; i++) {
            if (bMin[i] < min[i]) return false;
            else if (bMax[i] > max[i]) return false;
        }
        return true;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->isEnclosedByVolume(self);
}

bool AxisAlignedBoundingBox::isEnclosedByVolume(BoundingVolume*& boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const float& bRadius = bSphere->getRadius();
        const vec3 bCenter = bSphere->getCenter();
        const vec3 bMin = bCenter - vec3(bRadius, bRadius, bRadius);
        const vec3 bMax = bCenter + vec3(bRadius, bRadius, bRadius);

        const vec3 center = getCenter();
        const vec3 min = center - halfExtents;
        const vec3 max = center + halfExtents;

        for (int i = 0; i < 3; i++) {
            if (bMin[i] > min[i]) return false;
            else if (bMax[i] < max[i]) return false;
        }
        return true;
    }
    // handle axis aligned bounding box here
    else if (const AxisAlignedBoundingBox* bAABB = dynamic_cast<AxisAlignedBoundingBox*>(boundingVolume)) {
        const vec3 bCenter = bAABB->getCenter();
        const vec3 bMin = bCenter - bAABB->getHalfExtents();
        const vec3 bMax = bCenter + bAABB->getHalfExtents();

        const vec3 center = getCenter();
        const vec3 min = center - halfExtents;
        const vec3 max = center + halfExtents;

        for (int i = 0; i < 3; i++) {
            if (bMin[i] > min[i]) return false;
            else if (bMax[i] < max[i]) return false;
        }
        return true;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->enclosesVolume(self);
}

void AxisAlignedBoundingBox::update(const Transform& newTransform) {
    const mat3& upperLeft = mat3(newTransform.getMatrix());
    for (int i = 0; i < 3; i++) {
        halfExtents[i] = 0.f;
        for (int j = 0; j < 3; j++) {
            halfExtents[i] += fabs(upperLeft[j][i] * initialHalfExtents[j]);
        }
    }

    SceneObject::update(newTransform);
}