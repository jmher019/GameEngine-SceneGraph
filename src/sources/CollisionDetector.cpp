#include <CollisionDetector.hpp>

bool CollisionDetector::isVolumeIntersectingVolume(
    BoundingVolume* boundingVolume1,
    BoundingVolume* boundingVolume2
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume1)) {
        return isSphereIntersectingVolume(bSphere, boundingVolume2);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume1)) {
        return isCapsuleIntersectingVolume(bCapsule, boundingVolume2);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume1)) {
        return isOBBIntersectingVolume(bObb, boundingVolume2);
    }

    return false;
}

bool CollisionDetector::isSphereIntersectingVolume(
    BoundingSphere* sphere,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return isSphereIntersectingSphere(sphere, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return isCapsuleIntersectingSphere(bCapsule, sphere);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return isOBBIntersectingSphere(bObb, sphere);
    }

    return false;
}

bool CollisionDetector::isCapsuleIntersectingVolume(
    BoundingCapsule* capsule,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return isCapsuleIntersectingSphere(capsule, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return isCapsuleIntersectingCapsule(capsule, bCapsule);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return isOBBIntersectingCapsule(bObb, capsule);
    }

    return false;
}

bool CollisionDetector::isOBBIntersectingVolume(
    OrientedBoundingBox* obb,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return isOBBIntersectingSphere(obb, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return isOBBIntersectingCapsule(obb, bCapsule);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return isOBBIntersectingOBB(obb, bObb);
    }

    return false;
}

bool CollisionDetector::isSphereIntersectingSphere(
    BoundingSphere* sphere1,
    BoundingSphere* sphere2
) noexcept {
    const vec3 center1 = move(sphere1->getCenter());
    const vec3 center2 = move(sphere2->getCenter());
    const vec3 offset = center1 - center2;
    const float dist2 = dot(offset, offset);
    const float radiusSum = sphere1->getActualRadius() + sphere2->getActualRadius();

    return dist2 - radiusSum * radiusSum <= GeometryUtils::epsilon;
}

bool CollisionDetector::isCapsuleIntersectingSphere(
    BoundingCapsule* capsule,
    BoundingSphere* sphere
) noexcept {
    const Line line = move(capsule->getActualLine());
    const GLfloat radius = move(capsule->getActualRadius());

    const vec3 sphereCenter = move(sphere->getCenter());
    const vec3 capsuleLinePoint = move(line.getClosestPtPointSegment(sphereCenter));
    const vec3 offset = capsuleLinePoint - sphereCenter;
    const GLfloat radiusSum = radius + sphere->getActualRadius();
    const GLfloat dist2 = dot(offset, offset);

    return dist2 - radiusSum * radiusSum <= GeometryUtils::epsilon;
}

bool CollisionDetector::isCapsuleIntersectingCapsule(
    BoundingCapsule* capsule1,
    BoundingCapsule* capsule2
) noexcept {
    const Line line = move(capsule1->getActualLine());
    const GLfloat radius = move(capsule1->getActualRadius());

    const Line bLine = move(capsule2->getActualLine());

    vec3 c1, c2;
    const GLfloat dist2 = line.getClosestPtSegmentSegment(c1, c2, bLine);
    const GLfloat radiusSum = radius + capsule2->getActualRadius();

    return dist2 - radiusSum * radiusSum <= GeometryUtils::epsilon;
}

bool CollisionDetector::isOBBIntersectingSphere(
    OrientedBoundingBox* obb,
    BoundingSphere* sphere
) noexcept {
    const vec3 bCenter = move(sphere->getCenter());
    const vec3 closestPoint = move(GeometryUtils::getClosestPointBetweenPointAndOBB(
        bCenter,
        obb->getCenter(),
        obb->getXAxis(),
        obb->getYAxis(),
        obb->getZAxis(),
        obb->getActualHalfExtents()
    ));
    const vec3 diff = closestPoint - bCenter;
    const GLfloat dist2 = dot(diff, diff);
    const GLfloat bRadius = move(sphere->getActualRadius());

    return dist2 - bRadius * bRadius <= GeometryUtils::epsilon;
}

bool CollisionDetector::isOBBIntersectingCapsule(
    OrientedBoundingBox* obb,
    BoundingCapsule* capsule
) noexcept {
    vector<Triangle> triangles = move(obb->getTriangles());
    vec3 closestPointSegment, closestPointTriangle;
    const Line bLine = move(capsule->getActualLine());
    const vec3 bCenter = move(capsule->getCenter());
    GLfloat dist2 = triangles[0].getClosestPtSegmentTriangle(closestPointSegment, closestPointTriangle, bLine);
    for (size_t i = 1; i < triangles.size(); i++) {
        vec3 c1, c2;
        GLfloat currDist2 = triangles[i].getClosestPtSegmentTriangle(c1, c2, bLine);
        if (currDist2 < dist2) {
            dist2 = currDist2;
        }
    }

    const GLfloat bRadius = move(capsule->getActualRadius());
    return dist2 - bRadius * bRadius <= GeometryUtils::epsilon;
}

bool CollisionDetector::isOBBIntersectingOBB(
    OrientedBoundingBox* obb1,
    OrientedBoundingBox* obb2
) noexcept {
    GLfloat ra, rb;
    vector<vec3> bAxis;
    bAxis.push_back(move(obb2->getXAxis()));
    bAxis.push_back(move(obb2->getYAxis()));
    bAxis.push_back(move(obb2->getZAxis()));

    vector<vec3> axis;
    axis.push_back(move(obb1->getXAxis()));
    axis.push_back(move(obb1->getYAxis()));
    axis.push_back(move(obb1->getZAxis()));

    mat3 R, absR;
    const vec3 bHalfExtents = move(obb2->getActualHalfExtents());

    // compute rotation matrix expressing b in a's coordinate frame
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            R[i][j] = dot(axis[i], bAxis[j]);
        }
    }

    // compute translation vector t
    const vec3 center = move(obb1->getCenter());
    const vec3 bCenter = move(obb2->getCenter());
    vec3 t = bCenter - center;
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
    const vec3 halfExtents = move(obb1->getActualHalfExtents());
    for (int i = 0; i < 3; i++) {
        ra = halfExtents[i];
        rb = bHalfExtents[0] * absR[0][i] + bHalfExtents[1] * absR[1][i] + bHalfExtents[2] * absR[2][i];
        if (fabs(t[i]) > ra + rb) {
            return false;
        }
    }

    // Test axes L = B0, L = B1, L = B2
    for (int i = 0; i < 3; i++) {
        ra = halfExtents[0] * absR[i][0] + halfExtents[1] * absR[i][1] + halfExtents[2] * absR[i][2];
        rb = bHalfExtents[i];
        if (fabs(t[0] * R[i][0] + t[1] * R[i][1] + t[2] * R[i][2]) > ra + rb) {
            return false;
        }
    }

    // Test axis L = A0 x B0
    ra = halfExtents[1] * absR[0][2] + halfExtents[2] * absR[0][1];
    rb = bHalfExtents[1] * absR[2][0] + bHalfExtents[2] * absR[1][0];
    if (fabs(t[2] * R[0][1] - t[1] * R[0][2]) > ra + rb) {
        return false;
    }

    // Test axis L = A0 x B1
    ra = halfExtents[1] * absR[1][2] + halfExtents[2] * absR[1][1];
    rb = bHalfExtents[1] * absR[2][0] + bHalfExtents[2] * absR[0][0];
    if (fabs(t[2] * R[1][1] - t[1] * R[1][2]) > ra + rb) {
        return false;
    }

    // Test axis L = A0 X B2
    ra = halfExtents[1] * absR[2][2] + halfExtents[2] * absR[2][1];
    rb = bHalfExtents[0] * absR[1][0] + bHalfExtents[1] * absR[0][0];
    if (fabs(t[2] * R[2][1] - t[1] * R[2][2]) > ra + rb) {
        return false;
    }

    // Test axis L = A1 X B0
    ra = halfExtents[0] * absR[0][2] + halfExtents[2] * absR[0][0];
    rb = bHalfExtents[1] * absR[2][1] + bHalfExtents[2] * absR[1][1];
    if (fabs(t[0] * R[0][2] - t[2] * R[0][0]) > ra + rb) {
        return false;
    }

    // Test axis L = A1 X B1
    ra = halfExtents[0] * absR[1][2] + halfExtents[2] * absR[1][0];
    rb = bHalfExtents[0] * absR[2][1] + bHalfExtents[2] * absR[0][1];
    if (fabs(t[0] * R[1][2] - t[2] * R[1][0]) > ra + rb) {
        return false;
    }

    // Test axis L = A1 X B2
    ra = halfExtents[0] * absR[2][2] + halfExtents[2] * absR[2][0];
    rb = bHalfExtents[0] * absR[1][1] + bHalfExtents[1] * absR[0][1];
    if (fabs(t[0] * R[2][2] - t[2] * R[2][0]) > ra + rb) {
        return false;
    }

    // Test axis L = A2 X B0
    ra = halfExtents[0] * absR[0][1] + halfExtents[1] * absR[0][0];
    rb = bHalfExtents[1] * absR[2][2] + bHalfExtents[2] * absR[1][2];
    if (fabs(t[1] * R[0][0] - t[0] * R[0][1]) > ra + rb) {
        return false;
    }

    // Test axis L = A2 X B1
    ra = halfExtents[0] * absR[1][1] + halfExtents[1] * absR[1][0];
    rb = bHalfExtents[0] * absR[2][2] + bHalfExtents[2] * absR[0][2];
    if (fabs(t[1] * R[1][0] - t[0] * R[1][1]) > ra + rb) {
        return false;
    }

    // Test axis L = A2 X B2
    ra = halfExtents[0] * absR[2][1] + halfExtents[1] * absR[2][0];
    rb = bHalfExtents[0] * absR[1][2] + bHalfExtents[1] * absR[0][2];
    if (fabs(t[1] * R[2][0] - t[0] * R[2][1]) > ra + rb) {
        return false;
    }

    return true;
}

bool CollisionDetector::isVolumeEnclosingVolume(
    BoundingVolume* boundingVolume1,
    BoundingVolume* boundingVolume2
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume1)) {
        return isSphereEnclosingVolume(bSphere, boundingVolume2);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume1)) {
        return isCapsuleEnclosingVolume(bCapsule, boundingVolume2);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume1)) {
        return isOBBEnclosingVolume(bObb, boundingVolume2);
    }

    return false;
}

bool CollisionDetector::isSphereEnclosingVolume(
    BoundingSphere* sphere,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return isSphereEnclosingSphere(sphere, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return isSphereEnclosingCapsule(sphere, bCapsule);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return isSphereEnclosingOBB(sphere, bObb);
    }

    return false;
}

bool CollisionDetector::isCapsuleEnclosingVolume(
    BoundingCapsule* capsule,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return isCapsuleEnclosingSphere(capsule, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return isCapsuleEnclosingCapsule(capsule, bCapsule);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return isCapsuleEnclosingOBB(capsule, bObb);
    }

    return false;
}

bool CollisionDetector::isOBBEnclosingVolume(
    OrientedBoundingBox* obb,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return isOBBEnclosingSphere(obb, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return isOBBEnclosingCapsule(obb, bCapsule);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return isOBBEnclosingOBB(obb, bObb);
    }

    return false;
}

bool CollisionDetector::isSphereEnclosingSphere(
    BoundingSphere* sphere1,
    BoundingSphere* sphere2
) noexcept {
    const GLfloat radius = move(sphere1->getActualRadius());
    const vec3 diff = sphere1->getCenter() - sphere2->getCenter();
    const float fullDist = length(diff) + radius;
    const float& bRadius = move(sphere2->getActualRadius());

    return fullDist - bRadius <= GeometryUtils::epsilon;
}

bool CollisionDetector::isSphereEnclosingCapsule(
    BoundingSphere* sphere,
    BoundingCapsule* capsule
) noexcept {
    const Line line = move(capsule->getActualLine());
    const GLfloat radius = move(capsule->getActualRadius());

    const vec3 bCenter = move(sphere->getCenter());
    const vec3 diff1 = line.getClosestPtPointSegment(line.getPointStart()) - bCenter;
    const vec3 diff2 = line.getClosestPtPointSegment(line.getPointEnd()) - bCenter;
    const GLfloat fullDist1 = length(diff1) + radius;
    const GLfloat fullDist2 = length(diff2) + radius;
    const GLfloat bRadius = move(sphere->getActualRadius());

    return fullDist1 - bRadius <= GeometryUtils::epsilon && fullDist2 - bRadius <= GeometryUtils::epsilon;
}

bool CollisionDetector::isSphereEnclosingOBB(
    BoundingSphere* sphere,
    OrientedBoundingBox* obb
) noexcept {
    const vec3 center = move(obb->getCenter());
    const vec3 halfExtents = move(obb->getActualHalfExtents());
    const vec3 xLen = obb->getXAxis() * halfExtents.x;
    const vec3 yLen = obb->getYAxis() * halfExtents.y;
    const vec3 zLen = obb->getZAxis() * halfExtents.z;

    vector<vec3> testPoints;
    testPoints.push_back(-xLen - yLen - zLen);
    testPoints.push_back(-xLen - yLen + zLen);
    testPoints.push_back(xLen - yLen - zLen);
    testPoints.push_back(xLen - yLen + zLen);
    testPoints.push_back(xLen + yLen + zLen);
    testPoints.push_back(xLen + yLen - zLen);
    testPoints.push_back(-xLen + yLen + zLen);
    testPoints.push_back(-xLen + yLen - zLen);

    const vec3 t = center - sphere->getCenter();
    const GLfloat bRadius = move(sphere->getActualRadius());
    const GLfloat radius2 = bRadius * bRadius;
    for (size_t i = 0; i < testPoints.size(); i++) {
        const vec3& diff = testPoints[i] + t;

        if (dot(diff, diff) - radius2 > GeometryUtils::epsilon) {
            return false;
        }
    }

    return true;
}

bool CollisionDetector::isCapsuleEnclosingSphere(
    BoundingCapsule* capsule,
    BoundingSphere* sphere
) noexcept {
    const Line line = move(capsule->getActualLine());
    const GLfloat radius = move(capsule->getActualRadius());

    const vec3 bCenter = move(sphere->getCenter());
    const vec3 diff = line.getClosestPtPointSegment(bCenter) - bCenter;
    const GLfloat fullDist = length(diff) + sphere->getActualRadius();

    return fullDist - radius <= GeometryUtils::epsilon;
}

bool CollisionDetector::isCapsuleEnclosingCapsule(
    BoundingCapsule* capsule1,
    BoundingCapsule* capsule2
) noexcept {
    const Line line = move(capsule1->getActualLine());
    const GLfloat radius = move(capsule1->getActualRadius());

    const Line bLine = move(capsule2->getActualLine());
    const GLfloat bRadius = move(capsule2->getActualRadius());

    const vec3 diff1 = line.getClosestPtPointSegment(bLine.getPointStart()) - bLine.getPointStart();
    const vec3 diff2 = line.getClosestPtPointSegment(bLine.getPointEnd()) - bLine.getPointEnd();
    const GLfloat fullDist1 = length(diff1) + bRadius;
    const GLfloat fullDist2 = length(diff2) + bRadius;

    return fullDist1 - radius <= GeometryUtils::epsilon && fullDist2 - radius <= GeometryUtils::epsilon;
}

bool CollisionDetector::isCapsuleEnclosingOBB(
    BoundingCapsule* capsule,
    OrientedBoundingBox* obb
) noexcept {
    const vec3 center = move(obb->getCenter());
    const vec3 halfExtents = move(obb->getActualHalfExtents());
    const vec3 xLen = move(obb->getXAxis()) * halfExtents.x;
    const vec3 yLen = move(obb->getYAxis()) * halfExtents.y;
    const vec3 zLen = move(obb->getZAxis()) * halfExtents.z;

    vector<vec3> testPoints;
    testPoints.push_back(center - xLen - yLen - zLen);
    testPoints.push_back(center - xLen - yLen + zLen);
    testPoints.push_back(center + xLen - yLen - zLen);
    testPoints.push_back(center + xLen - yLen + zLen);
    testPoints.push_back(center + xLen + yLen + zLen);
    testPoints.push_back(center + xLen + yLen - zLen);
    testPoints.push_back(center - xLen + yLen + zLen);
    testPoints.push_back(center - xLen + yLen - zLen);

    const Line bLine = move(capsule->getActualLine());
    const GLfloat bRadius = move(capsule->getActualRadius());
    const GLfloat radius2 = bRadius * bRadius;
    for (size_t i = 0; i < testPoints.size(); i++) {
        const vec3& diff = bLine.getClosestPtPointSegment(testPoints[i]) - testPoints[i];

        if (dot(diff, diff) - radius2 > GeometryUtils::epsilon) {
            return false;
        }
    }

    return true;
}

bool CollisionDetector::isOBBEnclosingSphere(
    OrientedBoundingBox* obb,
    BoundingSphere* sphere
) noexcept {
    vector<vec3> axis;
    axis.push_back(move(obb->getXAxis()));
    axis.push_back(move(obb->getYAxis()));
    axis.push_back(move(obb->getZAxis()));

    vector<vec3> bAxis;
    mat4 bModel = (Transform(sphere->getTransform().getTranslationAndRotation())).getMatrix();
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
    vec3 t = sphere->getCenter() - obb->getCenter();
    // bring translation into a's coordinate frame
    t = vec3(dot(t, axis[0]), dot(t, axis[1]), dot(t, axis[2]));

    const GLfloat& bRadius = move(sphere->getActualRadius());
    vector<vec3> testPoints;
    testPoints.push_back(vec3(-bRadius, -bRadius, -bRadius));
    testPoints.push_back(vec3(-bRadius, -bRadius, bRadius));
    testPoints.push_back(vec3(bRadius, -bRadius, -bRadius));
    testPoints.push_back(vec3(bRadius, -bRadius, bRadius));
    testPoints.push_back(vec3(bRadius, bRadius, bRadius));
    testPoints.push_back(vec3(bRadius, bRadius, -bRadius));
    testPoints.push_back(vec3(-bRadius, bRadius, bRadius));
    testPoints.push_back(vec3(-bRadius, bRadius, -bRadius));

    const vec3 halfExtents = move(obb->getActualHalfExtents());
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

bool CollisionDetector::isOBBEnclosingCapsule(
    OrientedBoundingBox* obb,
    BoundingCapsule* capsule
) noexcept {
    vector<vec3> axis;
    axis.push_back(move(obb->getXAxis()));
    axis.push_back(move(obb->getYAxis()));
    axis.push_back(move(obb->getZAxis()));

    vector<vec3> bAxis;
    mat4 bModel = (Transform(capsule->getTransform().getTranslationAndRotation())).getMatrix();
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
    const Line bLine = move(capsule->getActualLine());
    vec3 t1 = bLine.getPointStart() - obb->getCenter();
    vec3 t2 = bLine.getPointEnd() - obb->getCenter();
    // bring translation into a's coordinate frame
    t1 = vec3(dot(t1, axis[0]), dot(t1, axis[1]), dot(t1, axis[2]));
    t2 = vec3(dot(t2, axis[0]), dot(t2, axis[1]), dot(t2, axis[2]));

    const vec3& bPoint1 = bLine.getPointStart();
    const vec3& bPoint2 = bLine.getPointEnd();

    const GLfloat& bRadius = move(capsule->getActualRadius());
    vector<vec3> testPoints;
    testPoints.push_back(vec3(-bRadius, -bRadius, -bRadius));
    testPoints.push_back(vec3(-bRadius, -bRadius, bRadius));
    testPoints.push_back(vec3(bRadius, -bRadius, -bRadius));
    testPoints.push_back(vec3(bRadius, -bRadius, bRadius));
    testPoints.push_back(vec3(bRadius, bRadius, bRadius));
    testPoints.push_back(vec3(bRadius, bRadius, -bRadius));
    testPoints.push_back(vec3(-bRadius, bRadius, bRadius));
    testPoints.push_back(vec3(-bRadius, bRadius, -bRadius));

    const vec3 halfExtents = move(obb->getActualHalfExtents());
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

bool CollisionDetector::isOBBEnclosingOBB(
    OrientedBoundingBox* obb1,
    OrientedBoundingBox* obb2
) noexcept {
    vector<vec3> axis;
    axis.push_back(move(obb1->getXAxis()));
    axis.push_back(move(obb1->getYAxis()));
    axis.push_back(move(obb1->getZAxis()));

    vector<vec3> bAxis;
    bAxis.push_back(move(obb2->getXAxis()));
    bAxis.push_back(move(obb2->getYAxis()));
    bAxis.push_back(move(obb2->getZAxis()));

    // compute rotation matrix expressing b in a's coordinate frame
    mat3 R;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            R[i][j] = dot(axis[i], bAxis[j]);
        }
    }

    // compute translation vector t
    vec3 t = obb2->getCenter() - obb1->getCenter();
    // bring translation into a's coordinate frame
    t = vec3(dot(t, axis[0]), dot(t, axis[1]), dot(t, axis[2]));

    const vec3 bHalfExtents = move(obb2->getActualHalfExtents());

    vector<vec3> testPoints;
    testPoints.push_back(vec3(-bHalfExtents.x, -bHalfExtents.y, -bHalfExtents.z));
    testPoints.push_back(vec3(-bHalfExtents.x, -bHalfExtents.y, bHalfExtents.z));
    testPoints.push_back(vec3(bHalfExtents.x, -bHalfExtents.y, -bHalfExtents.z));
    testPoints.push_back(vec3(bHalfExtents.x, -bHalfExtents.y, bHalfExtents.z));
    testPoints.push_back(vec3(bHalfExtents.x, bHalfExtents.y, bHalfExtents.z));
    testPoints.push_back(vec3(bHalfExtents.x, bHalfExtents.y, -bHalfExtents.z));
    testPoints.push_back(vec3(-bHalfExtents.x, bHalfExtents.y, bHalfExtents.z));
    testPoints.push_back(vec3(-bHalfExtents.x, bHalfExtents.y, -bHalfExtents.z));

    const vec3 halfExtents = move(obb1->getActualHalfExtents());
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

Contact CollisionDetector::findContactBetweenVolumeAndVolume(
    BoundingVolume* boundingVolume1,
    BoundingVolume* boundingVolume2
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume1)) {
        return CollisionDetector::findContactBetweenSphereAndVolume(bSphere, boundingVolume2);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume1)) {
        return CollisionDetector::findContactBetweenCapsuleAndVolume(bCapsule, boundingVolume2);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume1)) {
        return CollisionDetector::findContactBetweenOBBAndVolume(bObb, boundingVolume2);
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::findContactBetweenSphereAndVolume(
    BoundingSphere* sphere,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return CollisionDetector::findContactBetweenSphereAndSphere(sphere, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return CollisionDetector::findContactBetweenSphereAndCapsule(sphere, bCapsule);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return CollisionDetector::findContactBetweenSphereAndOBB(sphere, bObb);
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::findContactBetweenCapsuleAndVolume(
    BoundingCapsule* capsule,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return CollisionDetector::findContactBetweenCapsuleAndSphere(capsule, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return CollisionDetector::findContactBetweenCapsuleAndCapsule(capsule, bCapsule);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return CollisionDetector::findContactBetweenCapsuleAndOBB(capsule, bObb);
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::findContactBetweenOBBAndVolume(
    OrientedBoundingBox* obb,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return CollisionDetector::findContactBetweenOBBAndSphere(obb, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return CollisionDetector::findContactBetweenOBBAndCapsule(obb, bCapsule);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return CollisionDetector::findContactBetweenOBBAndOBB(obb, bObb);
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::findContactBetweenSphereAndSphere(
    BoundingSphere* sphere1,
    BoundingSphere* sphere2
) noexcept {
    const vec3 center1 = move(sphere1->getCenter());
    const vec3 center2 = move(sphere2->getCenter());
    const vec3 offset = center1 - center2;
    const float dist = glm::sqrt(dot(offset, offset));
    const float radiusSum = sphere1->getActualRadius() + sphere2->getActualRadius();

    return Contact(
        center1 + offset * 0.5f,
        offset / dist,
        radiusSum - dist,
        ContactValidity::VALID
    );
}

Contact CollisionDetector::findContactBetweenSphereAndCapsule(
    BoundingSphere* sphere,
    BoundingCapsule* capsule
) noexcept {
    const Line line = move(capsule->getActualLine());
    const GLfloat radius = move(capsule->getActualRadius());

    const vec3 sphereCenter = move(sphere->getCenter());
    const vec3 capsuleLinePoint = move(line.getClosestPtPointSegment(sphereCenter));
    const vec3 offset = capsuleLinePoint - sphereCenter;
    const GLfloat radiusSum = radius + sphere->getActualRadius();
    const GLfloat dist = glm::sqrt(dot(offset, offset));

    return Contact(
        capsuleLinePoint - offset * 0.5f,
        -offset / dist,
        radiusSum - dist,
        ContactValidity::VALID
    );
}

Contact CollisionDetector::findContactBetweenSphereAndOBB(
    BoundingSphere* sphere,
    OrientedBoundingBox* obb
) noexcept {
    const vec3 sphereCenter = move(sphere->getCenter());
    const vec3 obbCenter = move(obb->getCenter());
    const vec3 obbXAxis = move(obb->getXAxis());
    const vec3 obbYAxis = move(obb->getYAxis());
    const vec3 obbZAxis = move(obb->getZAxis());
    const vec3 obbHalfExtents = move(obb->getActualHalfExtents());
    const vec3 closestPoint = move(GeometryUtils::getClosestPointBetweenPointAndOBB(
        sphereCenter,
        obbCenter,
        obbXAxis,
        obbYAxis,
        obbZAxis,
        obbHalfExtents
    ));

    Contact contact = move(GeometryUtils::calculateContactBetweenOBBVertexAndOBB(
        closestPoint,
        obbCenter,
        obbXAxis,
        obbYAxis,
        obbZAxis,
        obbHalfExtents
    ));
    contact.setContactNormal(-contact.getContactNormal());
    return contact;
}

Contact CollisionDetector::findContactBetweenCapsuleAndSphere(
    BoundingCapsule* capsule,
    BoundingSphere* sphere
) noexcept {
    Contact contact = move(CollisionDetector::findContactBetweenSphereAndCapsule(sphere, capsule));
    contact.setContactNormal(-contact.getContactNormal());
    return contact;
}

Contact CollisionDetector::findContactBetweenCapsuleAndCapsule(
    BoundingCapsule* capsule1,
    BoundingCapsule* capsule2
) noexcept {
    const Line line = move(capsule1->getActualLine());
    const GLfloat radius = move(capsule1->getActualRadius());

    const Line bLine = move(capsule2->getActualLine());

    vec3 c1, c2;
    const GLfloat dist = glm::sqrt(line.getClosestPtSegmentSegment(c1, c2, bLine));
    const GLfloat radiusSum = radius + capsule2->getActualRadius();
    const vec3 offset = c1 - c2;

    return Contact(
        c1 + offset * 0.5f,
        offset / dist,
        radiusSum - dist,
        ContactValidity::VALID
    );
}

Contact CollisionDetector::findContactBetweenCapsuleAndOBB(
    BoundingCapsule* capsule,
    OrientedBoundingBox* obb
) noexcept {
    vector<Triangle> triangles = move(obb->getTriangles());
    vec3 closestPointSegment, closestPointTriangle;
    const Line line = move(capsule->getActualLine());
    const vec3 capsuleCenter = move(capsule->getCenter());
    GLfloat dist2 = triangles[0].getClosestPtSegmentTriangle(closestPointSegment, closestPointTriangle, line);
    for (size_t i = 1; i < triangles.size(); i++) {
        vec3 c1, c2;
        GLfloat currDist2 = triangles[i].getClosestPtSegmentTriangle(c1, c2, line);
        if (currDist2 < dist2) {
            dist2 = currDist2;
            closestPointTriangle = c2;
        }
    }

    Contact contact = move(GeometryUtils::calculateContactBetweenOBBVertexAndOBB(
        closestPointTriangle,
        obb->getCenter(),
        obb->getXAxis(),
        obb->getYAxis(),
        obb->getZAxis(),
        obb->getActualHalfExtents()
    ));
    contact.setContactNormal(-contact.getContactNormal());
    return contact;
}

Contact CollisionDetector::findContactBetweenOBBAndSphere(
    OrientedBoundingBox* obb,
    BoundingSphere* sphere
) noexcept {
    Contact contact = move(CollisionDetector::findContactBetweenSphereAndOBB(sphere, obb));
    contact.setContactNormal(-contact.getContactNormal());
    return contact;
}

Contact CollisionDetector::findContactBetweenOBBAndCapsule(
    OrientedBoundingBox* obb,
    BoundingCapsule* capsule
) noexcept {
    Contact contact = move(CollisionDetector::findContactBetweenCapsuleAndOBB(capsule, obb));
    contact.setContactNormal(-contact.getContactNormal());
    return contact;
}

Contact CollisionDetector::findContactBetweenOBBAndOBB(
    OrientedBoundingBox* obb1,
    OrientedBoundingBox* obb2
) noexcept {
    const vec3 center1 = move(obb1->getCenter());
    const vec3 halfExtents1 = move(obb1->getActualHalfExtents());
    vector<vec3> axis1;
    axis1.push_back(move(obb1->getXAxis()));
    axis1.push_back(move(obb1->getYAxis()));
    axis1.push_back(move(obb1->getZAxis()));

    const vec3 center2 = move(obb2->getCenter());
    const vec3 halfExtents2 = move(obb2->getActualHalfExtents());
    vector<vec3> axis2;
    axis2.push_back(move(obb2->getXAxis()));
    axis2.push_back(move(obb2->getYAxis()));
    axis2.push_back(move(obb2->getZAxis()));

    // Let's compute the contact point
    // Let's do point to face contact
    const vec3 xVector1 = halfExtents1.x * axis1[0];
    const vec3 yVector1 = halfExtents1.y * axis1[1];
    const vec3 zVector1 = halfExtents1.z * axis1[2];
    vector<vec3> corners1 = vector<vec3>(8, vec3());
    corners1[0] = center1 - xVector1 - yVector1 - zVector1;
    corners1[1] = center1 - xVector1 - yVector1 + zVector1;
    corners1[2] = center1 - xVector1 + yVector1 - zVector1;
    corners1[3] = center1 - xVector1 + yVector1 + zVector1;
    corners1[4] = center1 + xVector1 - yVector1 - zVector1;
    corners1[5] = center1 + xVector1 - yVector1 + zVector1;
    corners1[6] = center1 + xVector1 + yVector1 - zVector1;
    corners1[7] = center1 + xVector1 + yVector1 + zVector1;

    const vec3 xVector2 = halfExtents2.x * axis2[0];
    const vec3 yVector2 = halfExtents2.y * axis2[1];
    const vec3 zVector2 = halfExtents2.z * axis2[2];
    vector<vec3> corners2 = vector<vec3>(8, vec3());
    corners2[0] = center2 - xVector2 - yVector2 - zVector2;
    corners2[1] = center2 - xVector2 - yVector2 + zVector2;
    corners2[2] = center2 - xVector2 + yVector2 - zVector2;
    corners2[3] = center2 - xVector2 + yVector2 + zVector2;
    corners2[4] = center2 + xVector2 - yVector2 - zVector2;
    corners2[5] = center2 + xVector2 - yVector2 + zVector2;
    corners2[6] = center2 + xVector2 + yVector2 - zVector2;
    corners2[7] = center2 + xVector2 + yVector2 + zVector2;

    Contact contactVertex = Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
    for (size_t i = 0; i < corners1.size(); i++) {
        const Contact newContactVertex1 = move(GeometryUtils::calculateContactBetweenOBBVertexAndOBB(
            corners1[i],
            center2,
            axis2[0],
            axis2[1],
            axis2[2],
            halfExtents2
        ));

        if (contactVertex.getPenetration() < newContactVertex1.getPenetration() || contactVertex.getContactValidity() == ContactValidity::INVALID) {
            contactVertex = newContactVertex1;
        }

        const Contact newContactVertex2 = move(GeometryUtils::calculateContactBetweenOBBVertexAndOBB(
            corners2[i],
            center1,
            axis1[0],
            axis1[1],
            axis1[2],
            halfExtents1
        ));

        if (contactVertex.getPenetration() < newContactVertex2.getPenetration() || contactVertex.getContactValidity() == ContactValidity::INVALID) {
            contactVertex = newContactVertex2;
            contactVertex.setContactNormal(-contactVertex.getContactNormal());
        }
    }

    // Let's do edge to edge contact
    vector<Line> edges1 = vector<Line>(12, Line(vec3(), vec3()));
    edges1[0] = Line(corners1[0], corners1[1]);
    edges1[1] = Line(corners1[0], corners1[2]);
    edges1[2] = Line(corners1[0], corners1[4]);
    edges1[3] = Line(corners1[3], corners1[1]);
    edges1[4] = Line(corners1[3], corners1[2]);
    edges1[5] = Line(corners1[3], corners1[7]);
    edges1[6] = Line(corners1[5], corners1[7]);
    edges1[7] = Line(corners1[5], corners1[4]);
    edges1[8] = Line(corners1[5], corners1[1]);
    edges1[9] = Line(corners1[6], corners1[7]);
    edges1[10] = Line(corners1[6], corners1[4]);
    edges1[11] = Line(corners1[6], corners1[2]);

    vector<Line> edges2 = vector<Line>(12, Line(vec3(), vec3()));
    edges2[0] = Line(corners2[0], corners2[1]);
    edges2[1] = Line(corners2[0], corners2[2]);
    edges2[2] = Line(corners2[0], corners2[4]);
    edges2[3] = Line(corners2[3], corners2[1]);
    edges2[4] = Line(corners2[3], corners2[2]);
    edges2[5] = Line(corners2[3], corners2[7]);
    edges2[6] = Line(corners2[5], corners2[7]);
    edges2[7] = Line(corners2[5], corners2[4]);
    edges2[8] = Line(corners2[5], corners2[1]);
    edges2[9] = Line(corners2[6], corners2[7]);
    edges2[10] = Line(corners2[6], corners2[4]);
    edges2[11] = Line(corners2[6], corners2[2]);

    Contact contactEdge = Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
    for (size_t i = 0; i < edges2.size(); i++) {
        for (size_t j = 0; j < edges1.size(); j++) {
            vec3 c1, c2;
            edges1[j].getClosestPtSegmentSegment(c1, c2, edges2[i]);
            vec3 offset1 = c1 - center1;
            vec3 offset2 = c2 - center1;

            if (dot(offset1, offset1) > dot(offset2, offset2)) {
                Contact newContactEdge = move(GeometryUtils::calculateContactBetweenOBBVertexAndOBB(
                    c2,
                    center1,
                    axis1[0],
                    axis1[1],
                    axis1[2],
                    halfExtents1
                ));
                if (contactEdge.getContactValidity() == ContactValidity::INVALID || contactEdge.getPenetration() < newContactEdge.getPenetration()) {
                    contactEdge = newContactEdge;
                }
            }
        }
    }

    // The winner of the 2 methods is the one with the highest penetration
    return contactEdge.getContactValidity() == ContactValidity::INVALID || contactVertex.getPenetration() >= contactEdge.getPenetration() ? contactVertex : contactEdge;
}