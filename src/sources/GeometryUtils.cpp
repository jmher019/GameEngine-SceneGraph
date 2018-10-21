#include <GeometryUtils.hpp>

vec3 GeometryUtils::getClosestPointToTestPointAndSegment(
    const vec3& testPoint,
    const vec3& start,
    const vec3& end
) noexcept {
    const vec3 ab = end - start;
    // Project p onto ab, but deferring divide by Dot(ab, ab)
    const GLfloat t = dot(testPoint - start, ab);
    if (t <= 0.0f) {
        // p projects outside the [start, end] interval, on the start side; clamp to start
        return vec3(start);
    }

    const GLfloat denom = dot(ab, ab); // Always nonnegative since denom = ||ab||^2
    if (t >= denom) {
        // p projects outside the [start, end] interval, on the end side; clamp to end
        return vec3(end);
    }

    return start + ab * t / denom;
}

GLfloat GeometryUtils::getSquaredDistToTestPointAndSegment(
    const vec3& testPoint,
    const vec3& start,
    const vec3& end
) noexcept {
    const vec3 ab = end - start;
    const vec3 ap = testPoint - start;
    const vec3 bp = testPoint - end;
    const float e = dot(ap, ab);

    // Handle cases where c projects outside ab
    if (e <= 0.0f) {
        return dot(ap, ap);
    }

    const float f = dot(ab, ab);
    if (e >= f) {
        return dot(bp, bp);
    }

    // Handle cases where p projects onto ab
    return dot(ap, ap) - e * e / f;
}

GLfloat GeometryUtils::getClosestPointsBetweenSegments(
    vec3& c1,
    vec3& c2,
    const vec3& start1,
    const vec3& end1,
    const vec3& start2,
    const vec3& end2
) noexcept {
    const vec3 d1 = end1 - start1;
    const vec3 d2 = end2 - start2;
    const vec3 r = start1 - start2;
    const float b = dot(d1, d1);
    const float e = dot(d2, d2);
    const float f = dot(d2, r);
    float s = 0.f, t = 0.f;

    // Check if either or both segments degenerate into points
    if (b <= GeometryUtils::epsilon && e <= GeometryUtils::epsilon) {
        // Both segments degenerate into points
        c1 = start1;
        c2 = start2;
        return dot(c2 - c1, c2 - c1);
    }

    if (b <= GeometryUtils::epsilon) {
        // First segment degenerates into a point
        t = clamp(f / e, 0.f, 1.f);
    }
    else {
        const float c = dot(d1, r);
        if (e <= GeometryUtils::epsilon) {
            // second segment degenerates to a point
            s = clamp(-c / b, 0.f, 1.f);
        }
        else {
            // The general nondegenerate case starts here
            const float d = dot(d1, d2);
            const float denom = b * e - d * d;

            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment s1. Else pick arbitrary s (here 0)
            if (denom != 0.f) {
                s = clamp((d * f - c * e) / denom, 0.f, 1.f);
            }
            else {
                s = 0.f;
            }

            t = (d * s + f) / e;

            // If t in [0, 1] done. Else clamp t, recompute s for the new value of t
            if (t < 0.f) {
                t = 0.f;
                s = clamp(-c / b, 0.f, 1.f);
            }
            else if (t > 1.f) {
                t = 1.f;
                s = clamp((d - c) / b, 0.f, 1.f);
            }
        }
    }

    c1 = start1 + d1 * s;
    c2 = start2 + d2 * t;
    return dot(c2 - c1, c2 - c1);
}

vec3 GeometryUtils::computeNormalFromPoints(
    const vec3& point1,
    const vec3& point2,
    const vec3& point3
) noexcept {
    return cross(point2 - point1, point3 - point2);
}

vec3 GeometryUtils::computeBarycentricCoord(
    const vec3& testPoint,
    const vec3& point1,
    const vec3& point2,
    const vec3& point3
) noexcept {
    const vec3 vec0 = point2 - point1;
    const vec3 vec1 = point3 - point1;
    const vec3 vec2 = testPoint - point1;
    const GLfloat d00 = dot(vec0, vec0);
    const GLfloat d01 = dot(vec0, vec1);
    const GLfloat d11 = dot(vec1, vec1);
    const GLfloat d20 = dot(vec2, vec0);
    const GLfloat d21 = dot(vec2, vec1);
    const GLfloat denom = d00 * d11 - d01 * d01;

    const GLfloat v = (d11 * d20 - d01 * d21) / denom;
    const GLfloat w = (d00 * d21 - d01 * d20) / denom;
    return vec3(1.0f - v - w, v, w);
}

bool GeometryUtils::isPointWithinTriangle(
    const vec3& testPoint,
    const vec3& point1,
    const vec3& point2,
    const vec3& point3
) noexcept {
    const vec3 barycentricCoord = GeometryUtils::computeBarycentricCoord(testPoint, point1, point2, point3);

    return barycentricCoord[1] >= 0.0f &&
        barycentricCoord[2] >= 0.0f &&
        (barycentricCoord[1] + barycentricCoord[2]) <= 1.0f;
}

vec3 GeometryUtils::getClosestPointBetweenPointAndTriangle(
    const vec3& testPoint,
    const vec3& point1,
    const vec3& point2,
    const vec3& point3
) noexcept {
    // Check if P in vertex region outside A
    const vec3 ab = point2 - point1;
    const vec3 ac = point3 - point1;
    const vec3 ap = testPoint - point1;
    const GLfloat d1 = dot(ab, ap);
    const GLfloat d2 = dot(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) {
        return point1; // barycentric coordinates (1,0,0)
    }

    // Check if P in vertex region outside B
    const vec3 bp = testPoint - point2;
    const GLfloat d3 = dot(ab, bp);
    const GLfloat d4 = dot(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) {
        return point2; // barycentric coordinates (0,1,0)
    }

    // Check if P in edge region of AB, if so return projection of P onto AB
    const GLfloat vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        const GLfloat v = d1 / (d1 - d3);
        return point1 + v * ab; // barycentric coordinates (1-v,v,0)
    }
    // Check if P in vertex region outside C
    const vec3 cp = testPoint - point3;
    const GLfloat d5 = dot(ab, cp);
    const GLfloat d6 = dot(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) {
        return point3; // barycentric coordinates (0,0,1)
    }

    // Check if P in edge region of AC, if so return projection of P onto AC
    const GLfloat vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        const GLfloat w = d2 / (d2 - d6);
        return point1 + w * ac; // barycentric coordinates (1-w,0,w)
    }
    // Check if P in edge region of BC, if so return projection of P onto BC
    const GLfloat va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        const GLfloat w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return point2 + w * (point3 - point2); // barycentric coordinates (0,1-w,w)
    }
    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    const GLfloat denom = 1.0f / (va + vb + vc);
    const GLfloat v = vb * denom;
    const GLfloat w = vc * denom;
    return point1 + ab * v + ac * w; // = u*v1 + v*b + w*c, u = va * denom = 1.0f-v-w
}

bool GeometryUtils::isSegmentIntersectingTriangle(
    vec3& c,
    const vec3& start,
    const vec3& end,
    const vec3& point1,
    const vec3& point2,
    const vec3& point3
) noexcept {
    const vec3 n = computeNormalFromPoints(point1, point2, point3);
    const GLfloat d = dot(point1, n);

    const vec3 lVec = end - start;
    const GLfloat len2 = dot(lVec, lVec);

    // if the line degenerates to a point
    if (len2 < GeometryUtils::epsilon) {
        c = start;
        return isPointWithinTriangle(c, point1, point2, point3);
    }

    const GLfloat t = dot(point1 - start, n) / dot(lVec, n);

    if (t < 0.f || t > 1.f) {
        return false;
    }

    c = t * lVec + start;
    return isPointWithinTriangle(c, point1, point2, point3);
}

GLfloat GeometryUtils::getClosestPointBetweenSegmentAndTriangle(
    vec3& c1,
    vec3& c2,
    const vec3& start,
    const vec3& end,
    const vec3& point1,
    const vec3& point2,
    const vec3& point3
) noexcept {
    if (isSegmentIntersectingTriangle(c1, start, end, point1, point2, point3)) {
        c2 = c1;
        return 0.f;
    }

    vector<pair<vec3, vec3>> pairs;
    // closest point between l and v1 -> v2 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    getClosestPointsBetweenSegments(pairs.back().second, pairs.back().first, start, end, point1, point2);

    // closest point between l and v2 -> v3 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    getClosestPointsBetweenSegments(pairs.back().second, pairs.back().first, start, end, point2, point3);

    // closest point between l and v3 -> v1 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    getClosestPointsBetweenSegments(pairs.back().second, pairs.back().first, start, end, point3, point1);

    // closest point from the first vertex in l to the plane containing the triangle
    const vec3 n = computeNormalFromPoints(point1, point2, point3);
    const float d = dot(point1, n);
    const vec3 pc = getClosestPointBetweenPointAndPlane(start, n, d);
    // check if that point is within the triangle
    if (isPointWithinTriangle(pc, point1, point2, point3)) {
        pairs.push_back(make_pair(start, pc));
    }

    // closest point from the second vertex in l to the plane containing the triangle
    const vec3 qc = getClosestPointBetweenPointAndPlane(end, n, d);
    // check if that point is within the triangle
    if (isPointWithinTriangle(qc, point1, point2, point3)) {
        pairs.push_back(make_pair(end, qc));
    }

    pair<vec3, vec3>& result = pairs[0];
    vec3 diff = result.second - result.first;
    float dist2 = dot(diff, diff);

    for (size_t i = 1; i < pairs.size(); i++) {
        diff = pairs[i].second - pairs[i].first;
        float currDist2 = dot(diff, diff);
        if (dist2 > currDist2) {
            dist2 = currDist2;
            result = pairs[i];
        }
    }

    c1 = result.first;
    c2 = result.second;
    return dist2;
}

vec3 GeometryUtils::getClosestPointBetweenPointAndPlane(
    const vec3& testPoint,
    const vec3& planeNormal,
    const GLfloat& d
) noexcept {
    const float t = (dot(planeNormal, testPoint) - d) / dot(planeNormal, planeNormal);
    return testPoint - t * planeNormal;
}

bool GeometryUtils::isPointOutsideOfPlane(
    const vec3& testPoint,
    const vec3& planeNormal,
    const GLfloat& d
) noexcept {
    return fabs(dot(testPoint, planeNormal) - d) > GeometryUtils::epsilon;
}

bool GeometryUtils::arePointsOnOppositeSidesOfPlane(
    const vec3& point1,
    const vec3& point2,
    const vec3& planeNormal,
    const GLfloat& d
) noexcept {
    const float signp1 = dot(vec4(point1, 1.f), vec4(planeNormal, d));
    const float signp2 = dot(vec4(point2, 1.f), vec4(planeNormal, d));

    return signp1 * signp2 < 0.f;
}

vec3 GeometryUtils::getClosestPointBetweenPointAndOBB(
    const vec3& testPoint,
    const vec3& obbCenter,
    const vec3& obbXAxis,
    const vec3& obbYAxis,
    const vec3& obbZAxis,
    const vec3& obbActualHalfExtents,
    vec3& planeNormal
) noexcept {
    planeNormal.x = 0.f;
    planeNormal.y = 0.f;
    planeNormal.z = 0.f;

    const vec3 d = testPoint - obbCenter;
    // Start result at center of box; kame steps from there
    vec3 result = obbCenter;

    // project d onto the transformed x-axis to get the distance
    // along the axis of d from the box center
    GLfloat distX = dot(d, obbXAxis);
    planeNormal = obbXAxis * (distX < 0.f ? -1.f : 1.f);
    GLfloat minDepth = obbActualHalfExtents.x - glm::abs(distX);
    if (distX > obbActualHalfExtents.x) {
        distX = obbActualHalfExtents.x;
    }
    else if (distX < -obbActualHalfExtents.x) {
        distX = -obbActualHalfExtents.x;
    }

    result += distX * obbXAxis;

    // project d onto the transformed y-axis to get the distance
    // along the axis of d from the box center
    GLfloat distY = dot(d, obbYAxis);
    GLfloat depth = obbActualHalfExtents.y - glm::abs(distY);
    if (depth < minDepth) {
        minDepth = depth;
        planeNormal = obbYAxis * (distY < 0.f ? -1.f : 1.f);
    }

    if (distY > obbActualHalfExtents.y) {
        distY = obbActualHalfExtents.y;
    }
    else if (distY < -obbActualHalfExtents.y) {
        distY = -obbActualHalfExtents.y;
    }

    result += distY * obbYAxis;

    // project d onto the transformed z-axis to get the distance
    // along the axis of d from the box center
    GLfloat distZ = dot(d, obbZAxis);
    depth = obbActualHalfExtents.z - glm::abs(distZ);
    if (depth < minDepth) {
        minDepth = depth;
        planeNormal = obbZAxis * (distZ < 0.f ? -1.f : 1.f);
    }

    if (distZ > obbActualHalfExtents.z) {
        distZ = obbActualHalfExtents.z;
    }
    else if (distZ < -obbActualHalfExtents.z) {
        distZ = -obbActualHalfExtents.z;
    }

    result += distZ * obbZAxis;
    return result;
}