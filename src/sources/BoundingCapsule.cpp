#include <BoundingCapsule.hpp>

BoundingCapsule::BoundingCapsule(const GLfloat& capsuleLineLength, const GLfloat& radius, const string& name, const Transform& transform):
    capsuleLineLength(capsuleLineLength),
    radius(radius),
    BoundingVolume(name, transform) {
    vertices = getVerticesForGrid();
    initialize();
}

BoundingCapsule::BoundingCapsule(const BoundingCapsule& capsule):
    capsuleLineLength(capsule.capsuleLineLength),
    radius(capsule.radius),
    BoundingVolume(name, transform) {
    vertices = capsule.vertices;
    children = capsule.children;
    initialize();
}

BoundingCapsule::BoundingCapsule(BoundingCapsule&& capsule) {
    name = move(capsule.name);
    transform = move(capsule.transform);
    children = move(capsule.children);
    VAO = move(capsule.VAO);
    VBO = move(capsule.VBO);
    capsuleLineLength = move(capsule.capsuleLineLength);
    radius = move(capsule.radius);
    vertices = move(capsule.vertices);
}

BoundingCapsule& BoundingCapsule::operator=(const BoundingCapsule& capsule) noexcept {
    name = capsule.name;
    transform = capsule.transform;
    children = capsule.children;
    capsuleLineLength = capsule.capsuleLineLength;
    radius = capsule.radius;
    vertices = capsule.vertices;

    updateVBO();

    return *this;
}

BoundingCapsule& BoundingCapsule::operator=(BoundingCapsule&& capsule) noexcept {
    deallocate();

    name = move(capsule.name);
    transform = move(capsule.transform);
    children = move(capsule.children);
    VAO = move(capsule.VAO);
    VBO = move(capsule.VBO);
    capsuleLineLength = move(capsule.capsuleLineLength);
    radius = move(capsule.radius);
    vertices = move(capsule.vertices);

    updateVBO();

    return *this;
}

vector<Vertex> BoundingCapsule::getVerticesForGrid(void) const noexcept {
    vector<Vertex> vertices = vector<Vertex>(BoundingCapsule::POINTS_PER_RING * 8 + 8, {
        vec3(0.f, 0.f, 0.f),
        vec3(1.f, 0.f, 0.f),
        vec3(1.f, 1.f, 1.f),
        vec2()
        });

    //constexpr GLfloat ANGLE_SPACING = 2.f * pi<GLfloat>() / static_cast<GLfloat>(BoundingCapsule::POINTS_PER_RING);
    //constexpr GLint DOUBLE_POINTS_PER_RING = BoundingCapsule::POINTS_PER_RING * 2;
    //constexpr GLint DOUBLE_POINTS_PER_RING_TIMES_TWO = DOUBLE_POINTS_PER_RING * 2;
    //constexpr GLint DOUBLE_POINTS_PER_RING_TIMES_THREE = DOUBLE_POINTS_PER_RING * 3;

    //const vec3 lineVector = 
    //const vec3 center = getCenter();
    //const vec3 bottomCenter = 

    //// ring centered along the x axis
    //mat3x3 R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(1.f, 0.f, 0.f)));
    //vertices[0].position = vec3(0.f, 0.f, radius);
    //vertices[1].position = R * vertices[0].position;
    //for (GLint i = 2; i < POINTS_PER_RING; i += 2) {
    //    vertices[i].position = vertices[i - 1].position;
    //    vertices[i + 1].position = R * vertices[i].position;
    //}

    //// ring centered along the y axis
    //R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(0.f, 1.f, 0.f)));
    //vertices[DOUBLE_POINTS_PER_RING].position = vec3(0.f, 0.f, radius);
    //vertices[DOUBLE_POINTS_PER_RING].normal = vec3(0.f, 1.f, 0.f);
    //vertices[DOUBLE_POINTS_PER_RING + 1].position = R * vertices[0].position;
    //vertices[DOUBLE_POINTS_PER_RING + 1].normal = vec3(0.f, 1.f, 0.f);
    //for (GLint i = DOUBLE_POINTS_PER_RING + 2; i < DOUBLE_POINTS_PER_RING_TIMES_TWO; i += 2) {
    //    vertices[i].position = vertices[i - 1].position;
    //    vertices[i].normal = vertices[i - 1].normal;
    //    vertices[i + 1].position = R * vertices[i].position;
    //    vertices[i + 1].normal = R * vertices[i].normal;
    //}

    //// ring centered along the z axis
    //R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(0.f, 0.f, 1.f)));
    //vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].position = vec3(0.f, radius, 0.f);
    //vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].normal = vec3(0.f, 0.f, 1.f);
    //vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO + 1].position = R * vertices[0].position;
    //vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO + 1].normal = vec3(0.f, 0.f, 1.f);
    //for (GLint i = DOUBLE_POINTS_PER_RING_TIMES_TWO + 2; i < DOUBLE_POINTS_PER_RING_TIMES_THREE; i += 2) {
    //    vertices[i].position = vertices[i - 1].position;
    //    vertices[i].normal = vertices[i - 1].normal;
    //    vertices[i + 1].position = R * vertices[i].position;
    //    vertices[i + 1].normal = R * vertices[i].normal;
    //}

    return vertices;
}

const GLfloat& BoundingCapsule::getCapsuleLineLength(void) const noexcept {
    return capsuleLineLength;
}

const GLfloat& BoundingCapsule::getRadius(void) const noexcept {
    return radius;
}

bool BoundingCapsule::intersectsVolume(BoundingVolume*& boundingVolume) const noexcept {
    mat4 matrix = transform.getMatrix();
    vec3 start = vec3(matrix * vec4(0.f, -capsuleLineLength * 0.5f, 0.f, 1.f));
    vec3 end = vec3(matrix * vec4(0.f, capsuleLineLength * 0.5f, 0.f, 1.f));
    Line line(start, end);

    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 bCenter = bSphere->getCenter();
        const vec3 diff = line.getClosestPtPointSegment(bCenter) - bCenter;
        const GLfloat radiusSum = radius + bSphere->getRadius();
        return dot(diff, diff) <= radiusSum * radiusSum;
    }
    // handle axis aligned bounding box here
    else if (const AxisAlignedBoundingBox* bAABB = dynamic_cast<AxisAlignedBoundingBox*>(boundingVolume)) {
        vector<Triangle> triangles = move(bAABB->getTriangles());
        vec3 c1, c2;
        float dist2 = triangles[0].getClosestPtSegmentTriangle(c1, c2, line);

        for (size_t i = 1; i < triangles.size(); i++) {
            float currDist2 = triangles[i].getClosestPtSegmentTriangle(c1, c2, line);
            if (currDist2 < dist2) {
                dist2 = currDist2;
            }
        }

        return dist2 <= radius * radius;
    }
    // handle bounding capsule here
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        mat4 bMatrix = bCapsule->getTransform().getMatrix();
        vec3 bStart = vec3(bMatrix * vec4(0.f, -bCapsule->getCapsuleLineLength() * 0.5f, 0.f, 1.f));
        vec3 bEnd = vec3(bMatrix * vec4(0.f, bCapsule->getCapsuleLineLength() * 0.5f, 0.f, 1.f));
        Line bLine(bStart, bEnd);

        vec3 c1, c2;
        const GLfloat dist2 = line.getClosestPtSegmentSegment(c1, c2, bLine);
        const GLfloat radiusSum = radius + bCapsule->getRadius();
        return dist2 <= radiusSum * radiusSum;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->intersectsVolume(self);
}

bool BoundingCapsule::enclosesVolume(BoundingVolume*& boundingVolume) const noexcept {
    mat4 matrix = transform.getMatrix();
    vec3 start = vec3(matrix * vec4(0.f, -capsuleLineLength * 0.5f, 0.f, 1.f));
    vec3 end = vec3(matrix * vec4(0.f, capsuleLineLength * 0.5f, 0.f, 1.f));
    Line line(start, end);

    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 bCenter = bSphere->getCenter();
        const vec3 diff = line.getClosestPtPointSegment(bCenter) - bCenter;
        const GLfloat fullDist = length(diff) + bSphere->getRadius();

        return fullDist <= radius;
    }
    // handle axis aligned bounding box
    else if (const AxisAlignedBoundingBox* bAABB = dynamic_cast<AxisAlignedBoundingBox*>(boundingVolume)) {
        const vec3 bCenter = bAABB->getCenter();
        const vec3 halfExtents = bAABB->getHalfExtents();

        // make sure that each point in the box is contiained in the capsule
        vector<vec3> pts;
        pts.push_back(bCenter - halfExtents);
        pts.push_back(bCenter + vec3(-halfExtents.x, -halfExtents.y, halfExtents.z));
        pts.push_back(bCenter + vec3(halfExtents.x, -halfExtents.y, -halfExtents.z));
        pts.push_back(bCenter + vec3(halfExtents.x, -halfExtents.y, halfExtents.z));
        pts.push_back(bCenter + halfExtents);
        pts.push_back(bCenter + vec3(halfExtents.x, halfExtents.y, -halfExtents.z));
        pts.push_back(bCenter + vec3(-halfExtents.x, halfExtents.y, halfExtents.z));
        pts.push_back(bCenter + vec3(-halfExtents.x, halfExtents.y, -halfExtents.z));

        for (size_t i = 0; i < pts.size(); i++) {
            const vec3 diff = line.getClosestPtPointSegment(pts[i]) - pts[i];
            if (dot(diff, diff) > radius * radius) {
                return false;
            }
        }

        return true;
    }
    // handle bounding capsule
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        mat4 bMatrix = bCapsule->getTransform().getMatrix();
        vec3 bStart = vec3(bMatrix * vec4(0.f, -bCapsule->getCapsuleLineLength() * 0.5f, 0.f, 1.f));
        vec3 bEnd = vec3(bMatrix * vec4(0.f, bCapsule->getCapsuleLineLength() * 0.5f, 0.f, 1.f));
        
        const vec3 diff1 = line.getClosestPtPointSegment(bStart) - bStart;
        const vec3 diff2 = line.getClosestPtPointSegment(bEnd) - bEnd;
        const GLfloat fullDist1 = length(diff1) + bCapsule->getRadius();
        const GLfloat fullDist2 = length(diff2) + bCapsule->getRadius();

        return fullDist1 <= radius && fullDist2 <= radius;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->isEnclosedByVolume(self);
}

bool BoundingCapsule::isEnclosedByVolume(BoundingVolume*& boundingVolume) const noexcept {
    vec4 bottomCenter = vec4(0.f, -capsuleLineLength * 0.5f, 0.f, 1.f);
    vec4 topCenter = vec4(0.f, capsuleLineLength * 0.5f, 0.f, 1.f);
    vec4 xRadiusPoint = vec4(radius, 0.f, 0.f, 0.f) + bottomCenter;
    vec4 zRadiusPoint = vec4(0.f, 0.f, radius, 0.f) + bottomCenter;
    
    mat4 matrix = transform.getMatrix();
    vec3 start = vec3(matrix * bottomCenter);
    vec3 end = vec3(matrix * topCenter);
    Line line(start, end);

    vec3 transformedXRadiusPoint = vec3(matrix * xRadiusPoint);
    vec3 transformedZRadiusPoint = vec3(matrix * zRadiusPoint);

    vec3 xRadiusVector = transformedXRadiusPoint - start;
    vec3 zRadiusVector = transformedZRadiusPoint - start;

    GLfloat xRadiusSquared = dot(xRadiusVector, xRadiusVector);
    GLfloat zRadiusSquared = dot(zRadiusVector, zRadiusVector);

    GLfloat finalRadius = sqrtf(zRadiusSquared > xRadiusSquared ? zRadiusSquared : xRadiusSquared);

    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3& start = line.getPointStart();
        const vec3& end = line.getPointEnd();
        const vec3 bCenter = bSphere->getCenter();
        const vec3 diff1 = line.getClosestPtPointSegment(start) - bCenter;
        const vec3 diff2 = line.getClosestPtPointSegment(end) - bCenter;
        const GLfloat fullDist1 = length(diff1) + finalRadius;
        const GLfloat fullDist2 = length(diff2) + finalRadius;

        return fullDist1 <= bSphere->getRadius() && fullDist2 <= bSphere->getRadius();
    }
    // handle axis aligned bounding box
    else if (const AxisAlignedBoundingBox* bAABB = dynamic_cast<AxisAlignedBoundingBox*>(boundingVolume)) {
        const vec3 bCenter = bAABB->getCenter();
        const vec3 bMin = bCenter - bAABB->getHalfExtents();
        const vec3 bMax = bCenter + bAABB->getHalfExtents();

        const vec3& center1 = line.getPointStart();
        const vec3 min1 = center1 - vec3(finalRadius, finalRadius, finalRadius);
        const vec3 max1 = center1 + vec3(finalRadius, finalRadius, finalRadius);

        const vec3& center2 = line.getPointEnd();
        const vec3 min2 = center2 - vec3(finalRadius, finalRadius, finalRadius);
        const vec3 max2 = center2 + vec3(finalRadius, finalRadius, finalRadius);

        for (int i = 0; i < 3; i++) {
            if (bMin[i] > min1[i]) return false;
            else if (bMax[i] < max1[i]) return false;
        }

        for (int i = 0; i < 3; i++) {
            if (bMin[i] > min2[i]) return false;
            else if (bMax[i] < max2[i]) return false;
        }

        return true;
    }
    // handle bounding capsule
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        mat4 bMatrix = bCapsule->getTransform().getMatrix();
        vec3 bStart = vec3(bMatrix * vec4(0.f, -bCapsule->getCapsuleLineLength() * 0.5f, 0.f, 1.f));
        vec3 bEnd = vec3(bMatrix * vec4(0.f, bCapsule->getCapsuleLineLength() * 0.5f, 0.f, 1.f));
        Line bLine(bStart, bEnd);

        const vec3 diff1 = bLine.getClosestPtPointSegment(start) - start;
        const vec3 diff2 = bLine.getClosestPtPointSegment(end) - end;
        const GLfloat fullDist1 = length(diff1) + radius;
        const GLfloat fullDist2 = length(diff2) + radius;

        return fullDist1 <= bCapsule->getRadius() && fullDist2 <= bCapsule->getRadius();
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->enclosesVolume(self);
}

void BoundingCapsule::update(const Transform& newTransform) {
    SceneObject::update(newTransform);
}