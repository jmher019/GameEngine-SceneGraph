#ifndef GEOMETRY_UTILS_HPP
#define GEOMETRY_UTILS_HPP

#include <vector>

#include <Vertex.hpp>

class GeometryUtils {
public:
    static constexpr GLfloat epsilon = 1e-4f;

    static vec3 getClosestPointToTestPointAndSegment(
        const vec3& testPoint,
        const vec3& start,
        const vec3& end
    ) noexcept;

    static GLfloat getSquaredDistToTestPointAndSegment(
        const vec3& testPoint,
        const vec3& start,
        const vec3& end
    ) noexcept;

    static GLfloat getClosestPointsBetweenSegments(
        vec3& c1,
        vec3& c2,
        const vec3& start1,
        const vec3& end1,
        const vec3& start2,
        const vec3& end2
    ) noexcept;

    static vec3 computeNormalFromPoints(
        const vec3& point1,
        const vec3& point2,
        const vec3& point3
    ) noexcept;

    static vec3 computeBarycentricCoord(
        const vec3& testPoint,
        const vec3& point1,
        const vec3& point2,
        const vec3& point3
    ) noexcept;

    static vec3 getTriangleCenter(
        const vec3& point1,
        const vec3& point2,
        const vec3& point3
    ) noexcept;

    static bool isPointWithinTriangle(
        const vec3& testPoint,
        const vec3& point1,
        const vec3& point2,
        const vec3& point3
    ) noexcept;

    static vec3 getClosestPointBetweenPointAndTriangle(
        const vec3& testPoint,
        const vec3& point1,
        const vec3& point2,
        const vec3& point3
    ) noexcept;

    static bool isSegmentIntersectingTriangle(
        vec3& c,
        const vec3& start,
        const vec3& end,
        const vec3& point1,
        const vec3& point2,
        const vec3& point3
    ) noexcept;

    static GLfloat getClosestPointBetweenSegmentAndTriangle(
        vec3& c1,
        vec3& c2,
        const vec3& start,
        const vec3& end,
        const vec3& point1,
        const vec3& point2,
        const vec3& point3
    ) noexcept;

    static vec3 getClosestPointBetweenPointAndPlane(
        const vec3& testPoint,
        const vec3& planeNormal,
        const GLfloat& d
    ) noexcept;

    static bool isPointOutsideOfPlane(
        const vec3& testPoint,
        const vec3& planeNormal,
        const GLfloat& d
    ) noexcept;

    static bool arePointsOnOppositeSidesOfPlane(
        const vec3& point1,
        const vec3& point2,
        const vec3& planeNormal,
        const GLfloat& d
    ) noexcept;

    static vec3 getClosestPointBetweenPointAndOBB(
        const vec3& testPoint,
        const vec3& obbCenter,
        const vec3& obbXAxis,
        const vec3& obbYAxis,
        const vec3& obbZAxis,
        const vec3& obbActualHalfExtents,
        vec3& planeNormal
    ) noexcept;
};

#endif // !GEOMETRY_UTILS_HPP
