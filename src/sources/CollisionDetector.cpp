#include <CollisionDetector.hpp>

Contact CollisionDetector::isVolumeIntersectingVolume(
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

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::isSphereIntersectingVolume(
    BoundingSphere* sphere,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return isSphereIntersectingSphere(sphere, bSphere);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return isCapsuleIntersectingSphere(bCapsule, sphere, true);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return isOBBIntersectingSphere(bObb, sphere, true);
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::isCapsuleIntersectingVolume(
    BoundingCapsule* capsule,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return isCapsuleIntersectingSphere(capsule, bSphere, false);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return isCapsuleIntersectingCapsule(capsule, bCapsule);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return isOBBIntersectingCapsule(bObb, capsule, true);
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::isOBBIntersectingVolume(
    OrientedBoundingBox* obb,
    BoundingVolume* boundingVolume
) noexcept {
    if (BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        return isOBBIntersectingSphere(obb, bSphere, false);
    }
    else if (BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        return isOBBIntersectingCapsule(obb, bCapsule, false);
    }
    else if (OrientedBoundingBox* bObb = dynamic_cast<OrientedBoundingBox*>(boundingVolume)) {
        return isOBBIntersectingOBB(obb, bObb);
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::isSphereIntersectingSphere(
    BoundingSphere* sphere1,
    BoundingSphere* sphere2
) noexcept {
    const vec3 center1 = move(sphere1->getCenter());
    const vec3 center2 = move(sphere2->getCenter());
    const vec3 offset = center1 - center2;
    const float dist2 = dot(offset, offset);
    const float radiusSum = sphere1->getActualRadius() + sphere2->getActualRadius();

    if (dist2 - radiusSum * radiusSum <= GeometryUtils::epsilon) {
        const GLfloat dist = glm::sqrt(dist2);
        return Contact(
            center1 + offset * 0.5f,
            offset / dist,
            radiusSum - dist,
            ContactValidity::VALID
        );
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::isCapsuleIntersectingSphere(
    BoundingCapsule* capsule,
    BoundingSphere* sphere,
    GLboolean reverseContactTarget
) noexcept {
    const Line line = move(capsule->getActualLine());
    const GLfloat radius = move(capsule->getActualRadius());

    const vec3 sphereCenter = move(sphere->getCenter());
    const vec3 capsuleLinePoint = move(line.getClosestPtPointSegment(sphereCenter));
    const vec3 offset = capsuleLinePoint - sphereCenter;
    const GLfloat radiusSum = radius + sphere->getActualRadius();
    const GLfloat dist2 = dot(offset, offset);

    if (dist2 - radiusSum * radiusSum <= GeometryUtils::epsilon) {
        const GLfloat dist = glm::sqrt(dist2);
        return Contact(
            reverseContactTarget ? capsuleLinePoint + offset * 0.5f : sphereCenter - offset * 0.5f,
            reverseContactTarget ? offset / dist : -offset / dist,
            radiusSum - dist,
            ContactValidity::VALID
        );
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::isCapsuleIntersectingCapsule(
    BoundingCapsule* capsule1,
    BoundingCapsule* capsule2
) noexcept {
    const Line line = move(capsule1->getActualLine());
    const GLfloat radius = move(capsule1->getActualRadius());

    const Line bLine = move(capsule2->getActualLine());

    vec3 c1, c2;
    const GLfloat dist2 = line.getClosestPtSegmentSegment(c1, c2, bLine);
    const GLfloat radiusSum = radius + capsule2->getActualRadius();
    if (dist2 - radiusSum * radiusSum <= GeometryUtils::epsilon) {
        const GLfloat dist = glm::sqrt(dist2);
        const vec3 offset = c1 - c2;
        return Contact(
            c1 + offset * 0.5f,
            offset / dist,
            radiusSum - dist,
            ContactValidity::VALID
        );
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::isOBBIntersectingSphere(
    OrientedBoundingBox* obb,
    BoundingSphere* sphere,
    GLboolean reverseContactTarget
) noexcept {
    const vec3 bCenter = move(sphere->getCenter());
    vec3 normal;
    const vec3 closestPoint = move(GeometryUtils::getClosestPointBetweenPointAndOBB(
        bCenter,
        obb->getCenter(),
        obb->getXAxis(),
        obb->getYAxis(),
        obb->getZAxis(),
        obb->getActualHalfExtents(),
        normal
    ));
    const vec3 diff = closestPoint - bCenter;
    const GLfloat dist2 = dot(diff, diff);
    const GLfloat bRadius = move(sphere->getActualRadius());
    if (dist2 - bRadius * bRadius <= GeometryUtils::epsilon) {
        const GLfloat dist = glm::sqrt(dist2);
        return Contact(
            reverseContactTarget ? closestPoint + (closestPoint - bCenter) * 0.5f : bCenter + (bCenter - closestPoint) * 0.5f,
            reverseContactTarget ? normal : -normal,
            bRadius - dist,
            ContactValidity::VALID
        );
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::isOBBIntersectingCapsule(
    OrientedBoundingBox* obb,
    BoundingCapsule* capsule,
    GLboolean reverseContactTarget
) noexcept {
    vector<Triangle> triangles = move(obb->getTriangles());
    vec3 c1, c2;
    const Line bLine = move(capsule->getActualLine());
    GLfloat dist2 = triangles[0].getClosestPtSegmentTriangle(c1, c2, bLine);
    size_t closestTriangleIndex = 0;
    for (size_t i = 1; i < triangles.size(); i++) {
        GLfloat currDist2 = triangles[i].getClosestPtSegmentTriangle(c1, c2, bLine);
        if (currDist2 < dist2) {
            dist2 = currDist2;
            closestTriangleIndex = i;
        }
    }

    const GLfloat bRadius = move(capsule->getActualRadius());
    if (dist2 - bRadius * bRadius <= GeometryUtils::epsilon) {
        const GLfloat dist = glm::sqrt(dist2);
        return Contact(
            reverseContactTarget ? c1 + (c1 - c2) * 0.5f : c2 + (c2 - c1) * 0.5f,
            reverseContactTarget ? triangles[closestTriangleIndex].getNormal() : -triangles[closestTriangleIndex].getNormal(),
            bRadius - dist,
            ContactValidity::VALID
        );
    }

    return Contact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );
}

Contact CollisionDetector::isOBBIntersectingOBB(
    OrientedBoundingBox* obb1,
    OrientedBoundingBox* obb2
) noexcept {
    Contact invalidContact(
        vec3(0.f, 0.f, 0.f),
        vec3(0.f, 0.f, 0.f),
        0.f,
        ContactValidity::INVALID
    );

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
    const vec3& bHalfExtents = move(obb2->getActualHalfExtents());

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
            return invalidContact;
        }
    }

    // Test axes L = B0, L = B1, L = B2
    for (int i = 0; i < 3; i++) {
        ra = halfExtents[0] * absR[i][0] + halfExtents[1] * absR[i][1] + halfExtents[2] * absR[i][2];
        rb = bHalfExtents[i];
        if (fabs(t[0] * R[i][0] + t[1] * R[i][1] + t[2] * R[i][2]) > ra + rb) {
            return invalidContact;
        }
    }

    // Test axis L = A0 x B0
    ra = halfExtents[1] * absR[0][2] + halfExtents[2] * absR[0][1];
    rb = bHalfExtents[1] * absR[2][0] + bHalfExtents[2] * absR[1][0];
    if (fabs(t[2] * R[0][1] - t[1] * R[0][2]) > ra + rb) {
        return invalidContact;
    }

    // Test axis L = A0 x B1
    ra = halfExtents[1] * absR[1][2] + halfExtents[2] * absR[1][1];
    rb = bHalfExtents[1] * absR[2][0] + bHalfExtents[2] * absR[0][0];
    if (fabs(t[2] * R[1][1] - t[1] * R[1][2]) > ra + rb) {
        return invalidContact;
    }

    // Test axis L = A0 X B2
    ra = halfExtents[1] * absR[2][2] + halfExtents[2] * absR[2][1];
    rb = bHalfExtents[0] * absR[1][0] + bHalfExtents[1] * absR[0][0];
    if (fabs(t[2] * R[2][1] - t[1] * R[2][2]) > ra + rb) {
        return invalidContact;
    }

    // Test axis L = A1 X B0
    ra = halfExtents[0] * absR[0][2] + halfExtents[2] * absR[0][0];
    rb = bHalfExtents[1] * absR[2][1] + bHalfExtents[2] * absR[1][1];
    if (fabs(t[0] * R[0][2] - t[2] * R[0][0]) > ra + rb) {
        return invalidContact;
    }

    // Test axis L = A1 X B1
    ra = halfExtents[0] * absR[1][2] + halfExtents[2] * absR[1][0];
    rb = bHalfExtents[0] * absR[2][1] + bHalfExtents[2] * absR[0][1];
    if (fabs(t[0] * R[1][2] - t[2] * R[1][0]) > ra + rb) {
        return invalidContact;
    }

    // Test axis L = A1 X B2
    ra = halfExtents[0] * absR[2][2] + halfExtents[2] * absR[2][0];
    rb = bHalfExtents[0] * absR[1][1] + bHalfExtents[1] * absR[0][1];
    if (fabs(t[0] * R[2][2] - t[2] * R[2][0]) > ra + rb) {
        return invalidContact;
    }

    // Test axis L = A2 X B0
    ra = halfExtents[0] * absR[0][1] + halfExtents[1] * absR[0][0];
    rb = bHalfExtents[1] * absR[2][2] + bHalfExtents[2] * absR[1][2];
    if (fabs(t[1] * R[0][0] - t[0] * R[0][1]) > ra + rb) {
        return invalidContact;
    }

    // Test axis L = A2 X B1
    ra = halfExtents[0] * absR[1][1] + halfExtents[1] * absR[1][0];
    rb = bHalfExtents[0] * absR[2][2] + bHalfExtents[2] * absR[0][2];
    if (fabs(t[1] * R[1][0] - t[0] * R[1][1]) > ra + rb) {
        return invalidContact;
    }

    // Test axis L = A2 X B2
    ra = halfExtents[0] * absR[2][1] + halfExtents[1] * absR[2][0];
    rb = bHalfExtents[0] * absR[1][2] + bHalfExtents[1] * absR[0][2];
    if (fabs(t[1] * R[2][0] - t[0] * R[2][1]) > ra + rb) {
        return invalidContact;
    }

    // Let's compute the contact point
    // Let's do point to face contact
    const vec3 bXVector = bHalfExtents.x * bAxis[0];
    const vec3 bYVector = bHalfExtents.y * bAxis[1];
    const vec3 bZVector = bHalfExtents.z * bAxis[2];
    vector<vec3> bCorners = vector<vec3>(8, vec3());
    bCorners[0] = bCenter - bXVector - bYVector - bZVector;
    bCorners[1] = bCenter - bXVector - bYVector + bZVector;
    bCorners[2] = bCenter - bXVector + bYVector - bZVector;
    bCorners[3] = bCenter - bXVector + bYVector + bZVector;
    bCorners[4] = bCenter + bXVector - bYVector - bZVector;
    bCorners[5] = bCenter + bXVector - bYVector + bZVector;
    bCorners[6] = bCenter + bXVector + bYVector - bZVector;
    bCorners[7] = bCenter + bXVector + bYVector + bZVector;

    vec3 closestPointNormal;
    vec3 closestPointToCorner = GeometryUtils::getClosestPointBetweenPointAndOBB(
        bCorners[0],
        center,
        axis[0],
        axis[1],
        axis[2],
        halfExtents,
        closestPointNormal
    );
    vec3 pointToCenterOffset = closestPointToCorner - center;
    GLfloat pointToCenterOffsetDist2 = dot(pointToCenterOffset, pointToCenterOffset);
    vec3 closestCorner = bCorners[0];
    for (size_t i = 1; i < bCorners.size(); i++) {
        vec3 pointNormal;
        const vec3 closestPoint = GeometryUtils::getClosestPointBetweenPointAndOBB(
            bCorners[i],
            center,
            axis[0],
            axis[1],
            axis[2],
            halfExtents,
            pointNormal
        );
        const vec3 offset = closestPoint - center;
        const GLfloat dist2 = dot(offset, offset);

        if (dist2 < pointToCenterOffsetDist2) {
            closestPointNormal = pointNormal;
            closestPointToCorner = closestPoint;
            pointToCenterOffset = offset;
            pointToCenterOffsetDist2 = dist2;
            closestCorner = bCorners[i];
        }
    }

    // Let's do edge to edge contact
    const vec3 xVector = halfExtents.x * axis[0];
    const vec3 yVector = halfExtents.y * axis[1];
    const vec3 zVector = halfExtents.z * axis[2];
    vector<vec3> corners = vector<vec3>(8, vec3());
    corners[0] = center - xVector - yVector - zVector;
    corners[1] = center - xVector - yVector + zVector;
    corners[2] = center - xVector + yVector - zVector;
    corners[3] = center - xVector + yVector + zVector;
    corners[4] = center + xVector - yVector - zVector;
    corners[5] = center + xVector - yVector + zVector;
    corners[6] = center + xVector + yVector - zVector;
    corners[7] = center + xVector + yVector + zVector;

    vector<Line> edges = vector<Line>(12, Line(vec3(), vec3()));
    edges[0] = Line(corners[0], corners[1]);
    edges[1] = Line(corners[0], corners[2]);
    edges[2] = Line(corners[0], corners[4]);
    edges[3] = Line(corners[3], corners[1]);
    edges[4] = Line(corners[3], corners[2]);
    edges[5] = Line(corners[3], corners[7]);
    edges[6] = Line(corners[5], corners[7]);
    edges[7] = Line(corners[5], corners[4]);
    edges[8] = Line(corners[5], corners[1]);
    edges[9] = Line(corners[6], corners[7]);
    edges[10] = Line(corners[6], corners[4]);
    edges[11] = Line(corners[6], corners[2]);

    vector<Line> bEdges = vector<Line>(12, Line(vec3(), vec3()));
    bEdges[0] = Line(bCorners[0], bCorners[1]);
    bEdges[1] = Line(bCorners[0], bCorners[2]);
    bEdges[2] = Line(bCorners[0], bCorners[4]);
    bEdges[3] = Line(bCorners[3], bCorners[1]);
    bEdges[4] = Line(bCorners[3], bCorners[2]);
    bEdges[5] = Line(bCorners[3], bCorners[7]);
    bEdges[6] = Line(bCorners[5], bCorners[7]);
    bEdges[7] = Line(bCorners[5], bCorners[4]);
    bEdges[8] = Line(bCorners[5], bCorners[1]);
    bEdges[9] = Line(bCorners[6], bCorners[7]);
    bEdges[10] = Line(bCorners[6], bCorners[4]);
    bEdges[11] = Line(bCorners[6], bCorners[2]);

    vec3 closestPointEdge, bClosestPointEdge;
    Line bEdge = bEdges[0];
    Line edge = edges[0];
    edge.getClosestPtSegmentSegment(closestPointEdge, bClosestPointEdge, bEdge);
    vec3 segmentPointToCenter = bClosestPointEdge - center;
    GLfloat segmentPointToCenterDist2 = dot(segmentPointToCenter, segmentPointToCenter);
    for (size_t j = 1; j < edges.size(); j++) {
        Line& currentEdge = edges[j];
        vec3 c1, c2;
        currentEdge.getClosestPtSegmentSegment(c1, c2, bEdge);
        vec3 offset = c2 - center;
        GLfloat currentDist2 = dot(offset, offset);
        if (currentDist2 < segmentPointToCenterDist2) {
            edge = edges[j];
            closestPointEdge = c1;
            bClosestPointEdge = c2;
            segmentPointToCenter = offset;
            segmentPointToCenterDist2 = currentDist2;
        }
    }

    for (size_t i = 1; i < bEdges.size(); i++) {
        Line& currentBEdge = bEdges[i];
        for (size_t j = 0; j < edges.size(); j++) {
            Line& currentEdge = edges[j];
            vec3 c1, c2;
            currentEdge.getClosestPtSegmentSegment(c1, c2, bEdge);
            vec3 offset = c2 - center;
            GLfloat currentDist2 = dot(offset, offset);
            if (currentDist2 < segmentPointToCenterDist2) {
                bEdge = bEdges[i];
                edge = edges[j];
                closestPointEdge = c1;
                bClosestPointEdge = c2;
                segmentPointToCenter = offset;
                segmentPointToCenterDist2 = currentDist2;
            }
        }
    }

    // The winner of the 2 methods is the one that is closest to the center
    if (segmentPointToCenterDist2 < pointToCenterOffsetDist2) {
        const vec3 offset = closestPointEdge - bClosestPointEdge;
        const GLfloat dist = glm::length(offset);
        return Contact(
            closestPointEdge,
            offset / dist,
            dist,
            ContactValidity::VALID
        );
    }
    
    return Contact(
        closestPointToCorner,
        closestPointNormal,
        glm::length(closestPointToCorner - closestCorner),
        ContactValidity::VALID
    );
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