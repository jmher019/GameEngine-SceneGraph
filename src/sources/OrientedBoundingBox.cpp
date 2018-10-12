#include <OrientedBoundingBox.hpp>

OrientedBoundingBox::OrientedBoundingBox(const vec3& halfExtents, const string& name, const Transform& transform):
    halfExtents(halfExtents),
    BoundingVolume(name, transform) {
    vertices = getVerticesForGrid();
    initialize();
}

OrientedBoundingBox::OrientedBoundingBox(const OrientedBoundingBox& obb):
    halfExtents(obb.halfExtents),
    xAxis(obb.xAxis),
    yAxis(obb.yAxis),
    zAxis(obb.zAxis),
    BoundingVolume(obb.name, obb.transform) {
    vertices = obb.vertices;
    children = obb.children;
    initialize();
}

OrientedBoundingBox::OrientedBoundingBox(OrientedBoundingBox&& obb) {
    name = move(obb.name);
    transform = move(obb.transform);
    children = move(obb.children);
    VAO = move(obb.VAO);
    VBO = move(obb.VBO);
    halfExtents = move(obb.halfExtents);
    xAxis = move(obb.xAxis);
    yAxis = move(obb.yAxis);
    zAxis = move(obb.zAxis);
    vertices = move(obb.vertices);
}

OrientedBoundingBox& OrientedBoundingBox::operator=(const OrientedBoundingBox& obb) noexcept {
    name = obb.name;
    transform = obb.transform;
    children = obb.children;
    VAO = obb.VAO;
    VBO = obb.VBO;
    halfExtents = obb.halfExtents;
    xAxis = obb.xAxis;
    yAxis = obb.yAxis;
    zAxis = obb.zAxis;
    vertices = obb.vertices;

    updateVBO();

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
    xAxis = move(obb.xAxis);
    yAxis = move(obb.yAxis);
    zAxis = move(obb.zAxis);
    vertices = move(obb.vertices);

    updateVBO();

    return *this;
}

vector<Vertex> OrientedBoundingBox::getVerticesForGrid(void) const noexcept {
    vector<Vertex> vertices = vector<Vertex>(24, {
        vec3(0.f, 0.f, 0.f),
        vec3(1.f, 0.f, 0.f),
        vec3(1.f, 1.f, 1.f),
        vec2()
    });

    vertices[0].position = vec3(-halfExtents.x, halfExtents.y, halfExtents.z);
    vertices[1].position = vec3(halfExtents.x, halfExtents.y, halfExtents.z);
    vertices[2].position = vec3(halfExtents.x, halfExtents.y, halfExtents.z);
    vertices[3].position = vec3(halfExtents.x, -halfExtents.y, halfExtents.z);
    vertices[4].position = vec3(halfExtents.x, -halfExtents.y, halfExtents.z);
    vertices[5].position = vec3(-halfExtents.x, -halfExtents.y, halfExtents.z);
    vertices[6].position = vec3(-halfExtents.x, -halfExtents.y, halfExtents.z);
    vertices[7].position = vec3(-halfExtents.x, halfExtents.y, halfExtents.z);

    vertices[8].position = vec3(-halfExtents.x, halfExtents.y, -halfExtents.z);
    vertices[9].position = vec3(halfExtents.x, halfExtents.y, -halfExtents.z);
    vertices[10].position = vec3(halfExtents.x, halfExtents.y, -halfExtents.z);
    vertices[11].position = vec3(halfExtents.x, -halfExtents.y, -halfExtents.z);
    vertices[12].position = vec3(halfExtents.x, -halfExtents.y, -halfExtents.z);
    vertices[13].position = vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z);
    vertices[14].position = vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z);
    vertices[15].position = vec3(-halfExtents.x, halfExtents.y, -halfExtents.z);

    vertices[16].position = vec3(-halfExtents.x, halfExtents.y, halfExtents.z);
    vertices[17].position = vec3(-halfExtents.x, halfExtents.y, -halfExtents.z);
    vertices[18].position = vec3(halfExtents.x, halfExtents.y, halfExtents.z);
    vertices[19].position = vec3(halfExtents.x, halfExtents.y, -halfExtents.z);
    vertices[20].position = vec3(halfExtents.x, -halfExtents.y, halfExtents.z);
    vertices[21].position = vec3(halfExtents.x, -halfExtents.y, -halfExtents.z);
    vertices[22].position = vec3(-halfExtents.x, -halfExtents.y, halfExtents.z);
    vertices[23].position = vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z);

    return vertices;
}

const vec3& OrientedBoundingBox::getHalfExtents(void) const noexcept {
    return halfExtents;
}

const vec3& OrientedBoundingBox::getXAxis(void) const noexcept {
    return xAxis;
}

const vec3& OrientedBoundingBox::getYAxis(void) const noexcept {
    return yAxis;
}

const vec3& OrientedBoundingBox::getZAxis(void) const noexcept {
    return zAxis;
}

vector<Triangle> OrientedBoundingBox::getTriangles(void) const noexcept {
    vector<Triangle> triangles;

    const vec3 center = vec3(getCenter());
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

bool OrientedBoundingBox::intersectsVolume(BoundingVolume*& boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 bCenter = bSphere->getCenter();
        const vec3 diff = getClosestPtPointOBB(bCenter) - bCenter;
        const GLfloat dist2 = dot(diff, diff);
        return dist2 <= bSphere->getRadius() * bSphere->getRadius();
    }
    // handle axis aligned bounding box here
    else if (const AxisAlignedBoundingBox* bAABB = dynamic_cast<AxisAlignedBoundingBox*>(boundingVolume)) {
        GLfloat ra, rb;
        vector<vec3> bAxis;
        bAxis.push_back(vec3(1.f, 0.f, 0.f));
        bAxis.push_back(vec3(0.f, 1.f, 0.f));
        bAxis.push_back(vec3(0.f, 0.f, 1.f));

        vector<vec3> axis;
        axis.push_back(getXAxis());
        axis.push_back(getYAxis());
        axis.push_back(getZAxis());

        mat3 R, absR;
        const vec3& bHalfExtents = bAABB->getHalfExtents();

        // compute rotation matrix expressing b in a's coordinate frame
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[j][i] = dot(axis[i], bAxis[j]);
            }
        }

        // compute translation vector t
        vec3 t = vec3(bAABB->getCenter()) - vec3(getCenter());
        // bring translation into a's coordinate frame
        t = vec3(dot(t, axis[0]), dot(t, axis[1]), dot(t, axis[2]));

        // Compute common subexpressions. Add in an epsilon term to counteract arithmetic errors
        // when two edges are parallel and their cross product is (near) null
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                absR[j][i] = fabs(R[j][i]) + 1e-8f;
            }
        }

        // Test axes L = A0, L = A1, L = A2
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
    // handle bounding capsule here
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        /*vector<Triangle> triangles = move(getTriangles());
        vec3 c1, c2;
        GLfloat dist2 = triangles[0].getClosestPtSegmentTriangle(c1, c2, bCapsule->getLine());

        for (size_t i = 1; i < triangles.size(); i++) {
            GLfloat currDist2 = triangles[i].getClosestPtSegmentTriangle(c1, c2, bCapsule->getLine());
            if (currDist2 < dist2) {
                dist2 = currDist2;
            }
        }

        return dist2 <= bCapsule->getRadius() * bCapsule->getRadius();*/
    }
    // handle oriented bounding box here
    else if (const OrientedBoundingBox* bOBB = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        GLfloat ra, rb;
        vector<vec3> bAxis;
        bAxis.push_back(bOBB->getXAxis());
        bAxis.push_back(bOBB->getYAxis());
        bAxis.push_back(bOBB->getZAxis());

        vector<vec3> axis;
        axis.push_back(getXAxis());
        axis.push_back(getYAxis());
        axis.push_back(getZAxis());

        mat3 R, absR;
        const vec3& bHalfExtents = bOBB->getHalfExtents();

        // compute rotation matrix expressing b in a's coordinate frame
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[j][i] = dot(axis[i], bAxis[j]);
            }
        }

        // compute translation vector t
        vec3 t = vec3(bOBB->getCenter()) - vec3(getCenter());
        // bring translation into a's coordinate frame
        t = vec3(dot(t, axis[0]), dot(t, axis[1]), dot(t, axis[2]));

        // Compute common subexpressions. Add in an epsilon term to counteract arithmetic errors
        // when two edges are parallel and their cross product is (near) null
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                absR[j][i] = fabs(R[j][i]) + 1e-8f;
            }
        }

        // Test axes L = A0, L = A1, L = A2
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

bool OrientedBoundingBox::enclosesVolume(BoundingVolume*& boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        vector<vec3> axis;
        axis.push_back(getXAxis());
        axis.push_back(getYAxis());
        axis.push_back(getZAxis());

        vector<vec3> bAxis;
        bAxis.push_back(vec3(1.f, 0.f, 0.f));
        bAxis.push_back(vec3(0.f, 1.f, 0.f));
        bAxis.push_back(vec3(0.f, 0.f, 1.f));

        mat3 R;

        // compute rotation matrix expressing b in a's coordinate frame
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[j][i] = dot(axis[i], bAxis[j]);
            }
        }

        const GLfloat& bRadius = bSphere->getRadius();
        const vec3 bCenter = vec3(bSphere->getCenter());
        const vec3 bMin = bCenter - vec3(bRadius, bRadius, bRadius);
        const vec3 bMax = bCenter + vec3(bRadius, bRadius, bRadius);

        vector<vec3> testPoints;
        testPoints.push_back(bMin);
        testPoints.push_back(vec3(bMin.x, bMin.y, bMax.z));
        testPoints.push_back(vec3(bMax.x, bMin.y, bMin.z));
        testPoints.push_back(vec3(bMax.x, bMin.y, bMax.z));
        testPoints.push_back(vec3(bMax.x, bMax.y, bMax.z));
        testPoints.push_back(vec3(bMax.x, bMax.y, bMin.z));
        testPoints.push_back(vec3(bMin.x, bMax.y, bMax.z));
        testPoints.push_back(vec3(bMin.x, bMax.y, bMin.z));

        const vec3 center = vec3(getCenter());
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3 pt = R * (testPoints[i] - center);

            if (pt.x > halfExtents.x || pt.x < -halfExtents.x ||
                pt.y > halfExtents.y || pt.y < -halfExtents.y ||
                pt.z > halfExtents.z || pt.z < -halfExtents.z) {
                return false;
            }
        }

        return true;
    }
    // handle Axis Aligned Bounding Box here
    else if (const AxisAlignedBoundingBox* bAABB = dynamic_cast<AxisAlignedBoundingBox*>(boundingVolume)) {
        vector<vec3> axis;
        axis.push_back(getXAxis());
        axis.push_back(getYAxis());
        axis.push_back(getZAxis());

        vector<vec3> bAxis;
        bAxis.push_back(vec3(1.f, 0.f, 0.f));
        bAxis.push_back(vec3(0.f, 1.f, 0.f));
        bAxis.push_back(vec3(0.f, 0.f, 1.f));

        mat3 R;

        // compute rotation matrix expressing b in a's coordinate frame
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[j][i] = dot(axis[i], bAxis[j]);
            }
        }

        const vec3 bCenter = vec3(bAABB->getCenter());
        const vec3 bMin = bCenter - bAABB->getHalfExtents();
        const vec3 bMax = bCenter + bAABB->getHalfExtents();

        vector<vec3> testPoints;
        testPoints.push_back(bMin);
        testPoints.push_back(vec3(bMin.x, bMin.y, bMax.z));
        testPoints.push_back(vec3(bMax.x, bMin.y, bMin.z));
        testPoints.push_back(vec3(bMax.x, bMin.y, bMax.z));
        testPoints.push_back(vec3(bMax.x, bMax.y, bMax.z));
        testPoints.push_back(vec3(bMax.x, bMax.y, bMin.z));
        testPoints.push_back(vec3(bMin.x, bMax.y, bMax.z));
        testPoints.push_back(vec3(bMin.x, bMax.y, bMin.z));

        const vec3 center = vec3(getCenter());
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3 pt = R * (testPoints[i] - center);

            if (pt.x > halfExtents.x || pt.x < -halfExtents.x ||
                pt.y > halfExtents.y || pt.y < -halfExtents.y ||
                pt.z > halfExtents.z || pt.z < -halfExtents.z) {
                return false;
            }
        }

        return true;
    }
    // handle bounding capsule here
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        //vector<vec3> axis;
        //axis.push_back(getXAxis());
        //axis.push_back(getYAxis());
        //axis.push_back(getZAxis());

        //vector<vec3> bAxis;
        //bAxis.push_back(vec3(1.f, 0.f, 0.f));
        //bAxis.push_back(vec3(0.f, 1.f, 0.f));
        //bAxis.push_back(vec3(0.f, 0.f, 1.f));

        //mat3 R;

        //// compute rotation matrix expressing b in a's coordinate frame
        //for (int i = 0; i < 3; i++) {
        //    for (int j = 0; j < 3; j++) {
        //        R[j][i] = dot(axis[i], bAxis[j]);
        //    }
        //}

        //const GLfloat& bRadius = bCapsule->getRadius();
        //const vec3 bPoint1 = bCapsule->getLine().getPointStart();
        //const vec3 bPoint2 = bCapsule->getLine().getPointEnd();
        //const vec3 bMin1 = bPoint1 - vec3(bRadius, bRadius, bRadius);
        //const vec3 bMax1 = bPoint1 + vec3(bRadius, bRadius, bRadius);
        //const vec3 bMin2 = bPoint2 - vec3(bRadius, bRadius, bRadius);
        //const vec3 bMax2 = bPoint2 + vec3(bRadius, bRadius, bRadius);

        //vector<vec3> testPoints;
        //testPoints.push_back(bMin1);
        //testPoints.push_back(vec3(bMin1.x, bMin1.y, bMax1.z));
        //testPoints.push_back(vec3(bMax1.x, bMin1.y, bMin1.z));
        //testPoints.push_back(vec3(bMax1.x, bMin1.y, bMax1.z));
        //testPoints.push_back(vec3(bMax1.x, bMax1.y, bMax1.z));
        //testPoints.push_back(vec3(bMax1.x, bMax1.y, bMin1.z));
        //testPoints.push_back(vec3(bMin1.x, bMax1.y, bMax1.z));
        //testPoints.push_back(vec3(bMin1.x, bMax1.y, bMin1.z));

        //testPoints.push_back(bMin2);
        //testPoints.push_back(vec3(bMin2.x, bMin2.y, bMax2.z));
        //testPoints.push_back(vec3(bMax2.x, bMin2.y, bMin2.z));
        //testPoints.push_back(vec3(bMax2.x, bMin2.y, bMax2.z));
        //testPoints.push_back(vec3(bMax2.x, bMax2.y, bMax2.z));
        //testPoints.push_back(vec3(bMax2.x, bMax2.y, bMin2.z));
        //testPoints.push_back(vec3(bMin2.x, bMax2.y, bMax2.z));
        //testPoints.push_back(vec3(bMin2.x, bMax2.y, bMin2.z));

        //const vec3 center = vec3(getCenter());
        //for (size_t i = 0; i < testPoints.size(); i++) {
        //    const vec3 pt = R * (testPoints[i] - center);

        //    if (pt.x > halfExtents.x || pt.x < -halfExtents.x ||
        //        pt.y > halfExtents.y || pt.y < -halfExtents.y ||
        //        pt.z > halfExtents.z || pt.z < -halfExtents.z) {
        //        return false;
        //    }
        //}

        //return true;
    }
    // handle oriented bounding box here
    else if (const OrientedBoundingBox* bOBB = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        vector<vec3> axis;
        axis.push_back(getXAxis());
        axis.push_back(getYAxis());
        axis.push_back(getZAxis());

        vector<vec3> bAxis;
        bAxis.push_back(bOBB->getXAxis());
        bAxis.push_back(bOBB->getYAxis());
        bAxis.push_back(bOBB->getZAxis());

        mat3 R;

        // compute rotation matrix expressing b in a's coordinate frame
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[j][i] = dot(axis[i], bAxis[j]);
            }
        }

        const vec3 bCenter = vec3(bOBB->getCenter());
        const vec3& bHalfExtents = bOBB->getHalfExtents();
        const vec3 xLen = bAxis[0] * bHalfExtents.x;
        const vec3 yLen = bAxis[1] * bHalfExtents.y;
        const vec3 zLen = bAxis[2] * bHalfExtents.z;

        vector<vec3> testPoints;
        testPoints.push_back(bCenter - xLen - yLen - zLen);
        testPoints.push_back(bCenter - xLen - yLen + zLen);
        testPoints.push_back(bCenter + xLen - yLen - zLen);
        testPoints.push_back(bCenter + xLen - yLen + zLen);
        testPoints.push_back(bCenter + xLen + yLen + zLen);
        testPoints.push_back(bCenter + xLen + yLen - zLen);
        testPoints.push_back(bCenter - xLen + yLen + zLen);
        testPoints.push_back(bCenter - xLen + yLen - zLen);

        const vec3 center = vec3(getCenter());
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3 pt = R * (testPoints[i] - center);

            if (pt.x > halfExtents.x || pt.x < -halfExtents.x ||
                pt.y > halfExtents.y || pt.y < -halfExtents.y ||
                pt.z > halfExtents.z || pt.z < -halfExtents.z) {
                return false;
            }
        }

        return true;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->isEnclosedByVolume(self);
}

bool OrientedBoundingBox::isEnclosedByVolume(BoundingVolume*& boundingVolume) const noexcept {
    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 center = vec3(getCenter());
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

        const vec3 bCenter = vec3(bSphere->getCenter());
        const GLfloat radius2 = bSphere->getRadius() * bSphere->getRadius();
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3& diff = testPoints[i] - bCenter;

            if (dot(diff, diff) > radius2) {
                return false;
            }
        }

        return true;
    }
    // handle AxisAlignedBoundingBox
    else if (const AxisAlignedBoundingBox* bAABB = dynamic_cast<AxisAlignedBoundingBox*>(boundingVolume)) {
        const vec3 center = vec3(getCenter());
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

        const vec3 bCenter = vec3(bAABB->getCenter());
        const vec3& bHalfExtents = bAABB->getHalfExtents();
        const vec3 bMin = bCenter - bHalfExtents;
        const vec3 bMax = bCenter + bHalfExtents;

        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3& pt = testPoints[i];
            if (pt.x < bMin.x || pt.x > bMax.x ||
                pt.y < bMin.y || pt.y > bMax.y ||
                pt.z < bMin.z || pt.z > bMax.z) {
                return false;
            }
        }

        return true;
    }
    // handle bounding capsule
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        /*const vec3 center = vec3(getCenter());
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

        const Line bLine = bCapsule->getLine();
        const GLfloat radius2 = bCapsule->getRadius() * bCapsule->getRadius();
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3& diff = bLine.getClosestPtPointSegment(testPoints[i]) - testPoints[i];

            if (dot(diff, diff) > radius2) {
                return false;
            }
        }

        return true;*/
    }
    // handle oriented bounding box here
    else if (const OrientedBoundingBox* bOBB = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        vector<vec3> axis;
        axis.push_back(getXAxis());
        axis.push_back(getYAxis());
        axis.push_back(getZAxis());

        vector<vec3> bAxis;
        bAxis.push_back(bOBB->getXAxis());
        bAxis.push_back(bOBB->getYAxis());
        bAxis.push_back(bOBB->getZAxis());

        mat3 R;

        // compute rotation matrix expressing b in a's coordinate frame
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[j][i] = dot(bAxis[j], axis[i]);
            }
        }

        const vec3 center = vec3(getCenter());
        const vec3 xLen = axis[0] * halfExtents.x;
        const vec3 yLen = axis[1] * halfExtents.y;
        const vec3 zLen = axis[2] * halfExtents.z;

        vector<vec3> testPoints;
        testPoints.push_back(center - xLen - yLen - zLen);
        testPoints.push_back(center - xLen - yLen + zLen);
        testPoints.push_back(center + xLen - yLen - zLen);
        testPoints.push_back(center + xLen - yLen + zLen);
        testPoints.push_back(center + xLen + yLen + zLen);
        testPoints.push_back(center + xLen + yLen - zLen);
        testPoints.push_back(center - xLen + yLen + zLen);
        testPoints.push_back(center - xLen + yLen - zLen);

        const vec3 bCenter = vec3(bOBB->getCenter());
        const vec3& bHalfExtents = bOBB->getHalfExtents();
        for (size_t i = 0; i < testPoints.size(); i++) {
            const vec3 pt = R * (testPoints[i] - bCenter);

            if (pt.x > bHalfExtents.x || pt.x < -bHalfExtents.x ||
                pt.y > bHalfExtents.y || pt.y < -bHalfExtents.y ||
                pt.z > bHalfExtents.z || pt.z < -bHalfExtents.z) {
                return false;
            }
        }

        return true;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->enclosesVolume(self);
}

void OrientedBoundingBox::update(const Transform& newTransform) {
    SceneObject::update(newTransform);

    const mat3 upperLeft = mat3(newTransform.getMatrix());
    xAxis = upperLeft * xAxis;
    yAxis = upperLeft * yAxis;
    zAxis = upperLeft * zAxis;

    const float lengthX = length(xAxis);
    const float lengthY = length(yAxis);
    const float lengthZ = length(zAxis);

    halfExtents.x *= lengthX;
    halfExtents.y *= lengthY;
    halfExtents.z *= lengthZ;

    xAxis /= lengthX;
    yAxis /= lengthY;
    zAxis /= lengthZ;
}