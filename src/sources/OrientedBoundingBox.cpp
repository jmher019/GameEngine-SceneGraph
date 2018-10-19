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

vec3 OrientedBoundingBox::getClosestPtPointOBB(const vec3& pt) const noexcept {
    const vec3 center = getCenter();
    const vec3 d = pt - center;
    // Start result at center of box; kame steps from there
    vec3 result = center;

    const vec3 xTransformed = getXAxis();
    const vec3 yTransformed = getYAxis();
    const vec3 zTransformed = getZAxis();
    const vec3 halfExtents = getActualHalfExtents();

    // project d onto the transformed x-axis to get the distance
    // along the axis of d from the box center
    GLfloat distX = dot(d, xTransformed);
    if (distX > halfExtents.x) distX = halfExtents.x;
    else if (distX < -halfExtents.x) distX = halfExtents.x;

    result += distX * xTransformed;

    // project d onto the transformed y-axis to get the distance
    // along the axis of d from the box center
    GLfloat distY = dot(d, yTransformed);
    if (distY > halfExtents.y) distY = halfExtents.y;
    else if (distY < -halfExtents.y) distY = halfExtents.y;

    result += distY * yTransformed;

    // project d onto the transformed z-axis to get the distance
    // along the axis of d from the box center
    GLfloat distZ = dot(d, zTransformed);
    if (distZ > halfExtents.z) distZ = halfExtents.z;
    else if (distZ < -halfExtents.z) distZ = halfExtents.z;

    result += distZ * zTransformed;

    return result;
}

bool OrientedBoundingBox::intersectsVolume(BoundingVolume* boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 bCenter = bSphere->getCenter();
        const vec3 diff = getClosestPtPointOBB(bCenter) - bCenter;
        const GLfloat dist2 = dot(diff, diff);
        const GLfloat bRadius = bSphere->getActualRadius();
        return dist2 - bRadius * bRadius <= GeometryUtils::epsilon;
    }
    // handle bounding capsule here
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        vector<Triangle> triangles = move(getTriangles());
        vec3 c1, c2;
        Line bLine = move(bCapsule->getActualLine());
        GLfloat dist2 = triangles[0].getClosestPtSegmentTriangle(c1, c2, bLine);

        for (size_t i = 1; i < triangles.size(); i++) {
            GLfloat currDist2 = triangles[i].getClosestPtSegmentTriangle(c1, c2, bLine);
            if (currDist2 < dist2) {
                dist2 = currDist2;
            }
        }

        const GLfloat bRadius = bCapsule->getActualRadius();
        return dist2 - bRadius * bRadius <= GeometryUtils::epsilon;
    }
    // handle oriented bounding box here
    else if (const OrientedBoundingBox* bOBB = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        GLfloat ra, rb;
        vector<vec3> bAxis;
        bAxis.push_back(move(bOBB->getXAxis()));
        bAxis.push_back(move(bOBB->getYAxis()));
        bAxis.push_back(move(bOBB->getZAxis()));

        vector<vec3> axis;
        axis.push_back(move(getXAxis()));
        axis.push_back(move(getYAxis()));
        axis.push_back(move(getZAxis()));

        mat3 R, absR;
        const vec3& bHalfExtents = move(bOBB->getActualHalfExtents());

        // compute rotation matrix expressing b in a's coordinate frame
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[i][j] = dot(axis[i], bAxis[j]);
            }
        }

        // compute translation vector t
        vec3 t = bOBB->getCenter() - getCenter();
        // bring translation into a's coordinate frame
        t = vec3(dot(t, axis[0]), dot(t, axis[1]), dot(t, axis[2]));

        // Compute common subexpressions. Add in an epsilon term to counteract arithmetic errors
        // when two edges are parallel and their cross product is (near) null
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                absR[i][j] = glm::abs(R[i][j]) + GeometryUtils::epsilon;
            }
        }

        // Test axes L = A0, L = A1, L = A2
        const vec3 halfExtents = move(getActualHalfExtents());
        for (int i = 0; i < 3; i++) {
            ra = halfExtents[i];
            rb = bHalfExtents[0] * absR[0][i] + bHalfExtents[1] * absR[1][i] + bHalfExtents[2] * absR[2][i];
            if (fabs(t[i]) > ra + rb) return false;
        }

        // Test axes L = B0, L = B1, L = B2
        for (int i = 0; i < 3; i++) {
            ra = halfExtents[0] * absR[i][0] + halfExtents[1] * absR[i][1] + halfExtents[2] * absR[i][2];
            rb = bHalfExtents[i];
            if (fabs(t[0] * R[i][0] + t[1] * R[i][1] + t[2] * R[i][2]) > ra + rb) return false;
        }

        // Test axis L = A0 x B0
        ra = halfExtents[1] * absR[0][2] + halfExtents[2] * absR[0][1];
        rb = bHalfExtents[1] * absR[2][0] + bHalfExtents[2] * absR[1][0];
        if (fabs(t[2] * R[0][1] - t[1] * R[0][2]) > ra + rb) return false;

        // Test axis L = A0 x B1
        ra = halfExtents[1] * absR[1][2] + halfExtents[2] * absR[1][1];
        rb = bHalfExtents[1] * absR[2][0] + bHalfExtents[2] * absR[0][0];
        if (fabs(t[2] * R[1][1] - t[1] * R[1][2]) > ra + rb) return false;

        // Test axis L = A0 X B2
        ra = halfExtents[1] * absR[2][2] + halfExtents[2] * absR[2][1];
        rb = bHalfExtents[0] * absR[1][0] + bHalfExtents[1] * absR[0][0];
        if (fabs(t[2] * R[2][1] - t[1] * R[2][2]) > ra + rb) return false;

        // Test axis L = A1 X B0
        ra = halfExtents[0] * absR[0][2] + halfExtents[2] * absR[0][0];
        rb = bHalfExtents[1] * absR[2][1] + bHalfExtents[2] * absR[1][1];
        if (fabs(t[0] * R[0][2] - t[2] * R[0][0]) > ra + rb) return false;

        // Test axis L = A1 X B1
        ra = halfExtents[0] * absR[1][2] + halfExtents[2] * absR[1][0];
        rb = bHalfExtents[0] * absR[2][1] + bHalfExtents[2] * absR[0][1];
        if (fabs(t[0] * R[1][2] - t[2] * R[1][0]) > ra + rb) return false;

        // Test axis L = A1 X B2
        ra = halfExtents[0] * absR[2][2] + halfExtents[2] * absR[2][0];
        rb = bHalfExtents[0] * absR[1][1] + bHalfExtents[1] * absR[0][1];
        if (fabs(t[0] * R[2][2] - t[2] * R[2][0]) > ra + rb) return false;

        // Test axis L = A2 X B0
        ra = halfExtents[0] * absR[0][1] + halfExtents[1] * absR[0][0];
        rb = bHalfExtents[1] * absR[2][2] + bHalfExtents[2] * absR[1][2];
        if (fabs(t[1] * R[0][0] - t[0] * R[0][1]) > ra + rb) return false;

        // Test axis L = A2 X B1
        ra = halfExtents[0] * absR[1][1] + halfExtents[1] * absR[1][0];
        rb = bHalfExtents[0] * absR[2][2] + bHalfExtents[2] * absR[0][2];
        if (fabs(t[1] * R[1][0] - t[0] * R[1][1]) > ra + rb) return false;

        // Test axis L = A2 X B2
        ra = halfExtents[0] * absR[2][1] + halfExtents[1] * absR[2][0];
        rb = bHalfExtents[0] * absR[1][2] + bHalfExtents[1] * absR[0][2];
        if (fabs(t[1] * R[2][0] - t[0] * R[2][1]) > ra + rb) return false;

        return true;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->intersectsVolume(self);
}

bool OrientedBoundingBox::enclosesVolume(BoundingVolume* boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        vector<vec3> axis;
        axis.push_back(move(getXAxis()));
        axis.push_back(move(getYAxis()));
        axis.push_back(move(getZAxis()));

        vector<vec3> bAxis;
        mat4 bModel = (Transform(bSphere->getTransform().getTranslationAndRotation())).getMatrix();
        bAxis.push_back(normalize(vec3(bModel * vec4(1.f, 0.f, 0.f, 0.f))));
        bAxis.push_back(normalize(vec3(bModel * vec4(0.f, 1.f, 0.f, 0.f))));
        bAxis.push_back(normalize(vec3(bModel * vec4(0.f, 0.f, 1.f, 0.f))));

        // compute rotation matrix expressing b in a's coordinate frame
        mat3 R;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[i][j] = dot(axis[i], bAxis[j]);
            }
        }

        // compute translation vector t
        vec3 t = bSphere->getCenter() - getCenter();
        // bring translation into a's coordinate frame
        t = vec3(dot(t, axis[0]), dot(t, axis[1]), dot(t, axis[2]));

        const GLfloat& bRadius = bSphere->getActualRadius();
        vector<vec3> testPoints;
        testPoints.push_back(vec3(-bRadius, -bRadius, -bRadius));
        testPoints.push_back(vec3(-bRadius, -bRadius, bRadius));
        testPoints.push_back(vec3(bRadius, -bRadius, -bRadius));
        testPoints.push_back(vec3(bRadius, -bRadius, bRadius));
        testPoints.push_back(vec3(bRadius, bRadius, bRadius));
        testPoints.push_back(vec3(bRadius, bRadius, -bRadius));
        testPoints.push_back(vec3(-bRadius, bRadius, bRadius));
        testPoints.push_back(vec3(-bRadius, bRadius, -bRadius));

        const vec3 halfExtents = move(getActualHalfExtents());
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3 pt = t + R * testPoints[i];

            if (pt.x - halfExtents.x > GeometryUtils::epsilon ||
                pt.x + halfExtents.x < -GeometryUtils::epsilon ||
                pt.y - halfExtents.y > GeometryUtils::epsilon ||
                pt.y + halfExtents.y < -GeometryUtils::epsilon ||
                pt.z - halfExtents.z > GeometryUtils::epsilon ||
                pt.z + halfExtents.z < -GeometryUtils::epsilon) {
                return false;
            }
        }

        return true;
    }
    // handle bounding capsule here
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        vector<vec3> axis;
        axis.push_back(move(getXAxis()));
        axis.push_back(move(getYAxis()));
        axis.push_back(move(getZAxis()));

        vector<vec3> bAxis;
        mat4 bModel = (Transform(bCapsule->getTransform().getTranslationAndRotation())).getMatrix();
        bAxis.push_back(normalize(vec3(bModel * vec4(1.f, 0.f, 0.f, 0.f))));
        bAxis.push_back(normalize(vec3(bModel * vec4(0.f, 1.f, 0.f, 0.f))));
        bAxis.push_back(normalize(vec3(bModel * vec4(0.f, 0.f, 1.f, 0.f))));

        // compute rotation matrix expressing b in a's coordinate frame
        mat3 R;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[i][j] = dot(axis[i], bAxis[j]);
            }
        }

        // compute translation vector t
        const Line bLine = move(bCapsule->getActualLine());
        vec3 t1 = bLine.getPointStart() - getCenter();
        vec3 t2 = bLine.getPointEnd() - getCenter();
        // bring translation into a's coordinate frame
        t1 = vec3(dot(t1, axis[0]), dot(t1, axis[1]), dot(t1, axis[2]));
        t2 = vec3(dot(t2, axis[0]), dot(t2, axis[1]), dot(t2, axis[2]));
        
        const vec3& bPoint1 = bLine.getPointStart();
        const vec3& bPoint2 = bLine.getPointEnd();

        const GLfloat& bRadius = bCapsule->getActualRadius();
        vector<vec3> testPoints;
        testPoints.push_back(vec3(-bRadius, -bRadius, -bRadius));
        testPoints.push_back(vec3(-bRadius, -bRadius, bRadius));
        testPoints.push_back(vec3(bRadius, -bRadius, -bRadius));
        testPoints.push_back(vec3(bRadius, -bRadius, bRadius));
        testPoints.push_back(vec3(bRadius, bRadius, bRadius));
        testPoints.push_back(vec3(bRadius, bRadius, -bRadius));
        testPoints.push_back(vec3(-bRadius, bRadius, bRadius));
        testPoints.push_back(vec3(-bRadius, bRadius, -bRadius));

        const vec3 halfExtents = move(getActualHalfExtents());
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3 pt = t1 + R * testPoints[i];

            if (pt.x - halfExtents.x > GeometryUtils::epsilon ||
                pt.x + halfExtents.x < -GeometryUtils::epsilon ||
                pt.y - halfExtents.y > GeometryUtils::epsilon ||
                pt.y + halfExtents.y < -GeometryUtils::epsilon ||
                pt.z - halfExtents.z > GeometryUtils::epsilon ||
                pt.z + halfExtents.z < -GeometryUtils::epsilon) {
                return false;
            }
        }

        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3 pt = t2 + R * testPoints[i];

            if (pt.x - halfExtents.x > GeometryUtils::epsilon ||
                pt.x + halfExtents.x < -GeometryUtils::epsilon ||
                pt.y - halfExtents.y > GeometryUtils::epsilon ||
                pt.y + halfExtents.y < -GeometryUtils::epsilon ||
                pt.z - halfExtents.z > GeometryUtils::epsilon ||
                pt.z + halfExtents.z < -GeometryUtils::epsilon) {
                return false;
            }
        }

        return true;
    }
    // handle oriented bounding box here
    else if (const OrientedBoundingBox* bOBB = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        vector<vec3> axis;
        axis.push_back(move(getXAxis()));
        axis.push_back(move(getYAxis()));
        axis.push_back(move(getZAxis()));

        vector<vec3> bAxis;
        bAxis.push_back(move(bOBB->getXAxis()));
        bAxis.push_back(move(bOBB->getYAxis()));
        bAxis.push_back(move(bOBB->getZAxis()));

        // compute rotation matrix expressing b in a's coordinate frame
        mat3 R;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[i][j] = dot(axis[i], bAxis[j]);
            }
        }

        // compute translation vector t
        vec3 t = bOBB->getCenter() - getCenter();
        // bring translation into a's coordinate frame
        t = vec3(dot(t, axis[0]), dot(t, axis[1]), dot(t, axis[2]));

        const vec3 bHalfExtents = move(bOBB->getActualHalfExtents());

        vector<vec3> testPoints;
        testPoints.push_back(vec3(-bHalfExtents.x, -bHalfExtents.y, -bHalfExtents.z));
        testPoints.push_back(vec3(-bHalfExtents.x, -bHalfExtents.y, bHalfExtents.z));
        testPoints.push_back(vec3(bHalfExtents.x, -bHalfExtents.y, -bHalfExtents.z));
        testPoints.push_back(vec3(bHalfExtents.x, -bHalfExtents.y, bHalfExtents.z));
        testPoints.push_back(vec3(bHalfExtents.x, bHalfExtents.y, bHalfExtents.z));
        testPoints.push_back(vec3(bHalfExtents.x, bHalfExtents.y, -bHalfExtents.z));
        testPoints.push_back(vec3(-bHalfExtents.x, bHalfExtents.y, bHalfExtents.z));
        testPoints.push_back(vec3(-bHalfExtents.x, bHalfExtents.y, -bHalfExtents.z));

        const vec3 halfExtents = move(getActualHalfExtents());
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3 pt = t + R * testPoints[i];

            if (pt.x - halfExtents.x > GeometryUtils::epsilon ||
                pt.x + halfExtents.x < -GeometryUtils::epsilon ||
                pt.y - halfExtents.y > GeometryUtils::epsilon ||
                pt.y + halfExtents.y < -GeometryUtils::epsilon ||
                pt.z - halfExtents.z > GeometryUtils::epsilon ||
                pt.z + halfExtents.z < -GeometryUtils::epsilon) {
                return false;
            }
        }

        return true;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->isEnclosedByVolume(self);
}

bool OrientedBoundingBox::isEnclosedByVolume(BoundingVolume* boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 center = getCenter();
        const vec3 halfExtents = move(getActualHalfExtents());
        const vec3 xLen = getXAxis() * halfExtents.x;
        const vec3 yLen = getYAxis() * halfExtents.y;
        const vec3 zLen = getZAxis() * halfExtents.z;

        vector<vec3> testPoints;
        testPoints.push_back(-xLen - yLen - zLen);
        testPoints.push_back(-xLen - yLen + zLen);
        testPoints.push_back(xLen - yLen - zLen);
        testPoints.push_back(xLen - yLen + zLen);
        testPoints.push_back(xLen + yLen + zLen);
        testPoints.push_back(xLen + yLen - zLen);
        testPoints.push_back(-xLen + yLen + zLen);
        testPoints.push_back(-xLen + yLen - zLen);

        const vec3 t = center - bSphere->getCenter();
        const GLfloat bRadius = bSphere->getActualRadius();
        const GLfloat radius2 = bRadius * bRadius;
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3& diff = testPoints[i] + t;

            if (dot(diff, diff) - radius2 > GeometryUtils::epsilon) {
                return false;
            }
        }

        return true;
    }
    // handle bounding capsule
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        const vec3 center = getCenter();
        const vec3 halfExtents = move(getActualHalfExtents());
        const vec3 xLen = getXAxis() * halfExtents.x;
        const vec3 yLen = getYAxis() * halfExtents.y;
        const vec3 zLen = getZAxis() * halfExtents.z;

        vector<vec3> testPoints;
        testPoints.push_back(center - xLen - yLen - zLen);
        testPoints.push_back(center - xLen - yLen + zLen);
        testPoints.push_back(center + xLen - yLen - zLen);
        testPoints.push_back(center + xLen - yLen + zLen);
        testPoints.push_back(center + xLen + yLen + zLen);
        testPoints.push_back(center + xLen + yLen - zLen);
        testPoints.push_back(center - xLen + yLen + zLen);
        testPoints.push_back(center - xLen + yLen - zLen);

        const Line bLine = move(bCapsule->getActualLine());
        const GLfloat bRadius = bCapsule->getActualRadius();
        const GLfloat radius2 = bRadius * bRadius;
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3& diff = bLine.getClosestPtPointSegment(testPoints[i]) - testPoints[i];

            if (dot(diff, diff) - radius2 > GeometryUtils::epsilon) {
                return false;
            }
        }

        return true;
    }
    // handle oriented bounding box here
    else if (const OrientedBoundingBox* bOBB = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        vector<vec3> axis;
        axis.push_back(move(getXAxis()));
        axis.push_back(move(getYAxis()));
        axis.push_back(move(getZAxis()));

        vector<vec3> bAxis;
        bAxis.push_back(move(bOBB->getXAxis()));
        bAxis.push_back(move(bOBB->getYAxis()));
        bAxis.push_back(move(bOBB->getZAxis()));

        mat3 R;

        // compute rotation matrix expressing b in a's coordinate frame
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[i][j] = dot(bAxis[j], axis[i]);
            }
        }

        // compute translation vector t
        vec3 t = getCenter() - bOBB->getCenter();
        // bring translation into a's coordinate frame
        t = vec3(dot(t, bAxis[0]), dot(t, bAxis[1]), dot(t, bAxis[2]));

        const vec3 halfExtents = move(getActualHalfExtents());

        vector<vec3> testPoints;
        testPoints.push_back(vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z));
        testPoints.push_back(vec3(-halfExtents.x, -halfExtents.y, halfExtents.z));
        testPoints.push_back(vec3(halfExtents.x, -halfExtents.y, -halfExtents.z));
        testPoints.push_back(vec3(halfExtents.x, -halfExtents.y, halfExtents.z));
        testPoints.push_back(vec3(halfExtents.x, halfExtents.y, halfExtents.z));
        testPoints.push_back(vec3(halfExtents.x, halfExtents.y, -halfExtents.z));
        testPoints.push_back(vec3(-halfExtents.x, halfExtents.y, halfExtents.z));
        testPoints.push_back(vec3(-halfExtents.x, halfExtents.y, -halfExtents.z));

        const vec3& bHalfExtents = move(bOBB->getActualHalfExtents());
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3 pt = t + R * testPoints[i];

            if (pt.x - bHalfExtents.x > GeometryUtils::epsilon ||
                pt.x + bHalfExtents.x < -GeometryUtils::epsilon ||
                pt.y - bHalfExtents.y > GeometryUtils::epsilon ||
                pt.y + bHalfExtents.y < -GeometryUtils::epsilon ||
                pt.z - bHalfExtents.z > GeometryUtils::epsilon ||
                pt.z + bHalfExtents.z < -GeometryUtils::epsilon) {
                return false;
            }
        }

        return true;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->enclosesVolume(self);
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