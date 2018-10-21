#include <Triangle.hpp>

Triangle::Triangle(const vec3& v1, const vec3& v2, const vec3& v3):
    v1(v1),
    v2(v2),
    v3(v3) {
}

Triangle::Triangle(const Triangle& triangle):
    v1(triangle.v1),
    v2(triangle.v2), 
    v3(triangle.v3) {
}

Triangle::Triangle(Triangle&& triangle):
    v1(move(triangle.v1)),
    v2(move(triangle.v2)),
    v3(move(triangle.v3)) {
}

Triangle& Triangle::operator=(const Triangle& triangle) noexcept {
    v1 = triangle.v1;
    v2 = triangle.v2;
    v3 = triangle.v3;

    return *this;
}

Triangle& Triangle::operator=(Triangle&& triangle) noexcept {
    v1 = move(triangle.v1);
    v2 = move(triangle.v2);
    v3 = move(triangle.v3);

    return *this;
}

const vec3& Triangle::getVertex1(void) const noexcept {
    return v1;
}

void Triangle::setVertex1(const vec3& v1) noexcept {
    this->v1 = v1;
}

const vec3& Triangle::getVertex2(void) const noexcept {
    return v2;
}

void Triangle::setVertex2(const vec3& v2) noexcept {
    this->v2 = v2;
}

const vec3& Triangle::getVertex3(void) const noexcept {
    return v3;
}

void Triangle::setVertex3(const vec3& v3) noexcept {
    this->v3 = v3;
}

vec3 Triangle::getNormal(void) const noexcept {
    return GeometryUtils::computeNormalFromPoints(v1, v2, v3);
}

vec3 Triangle::getNormal(const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    return GeometryUtils::computeNormalFromPoints(v1, v2, v3);
}

vec3 Triangle::getBarycentricCoord(const vec3& p) const noexcept {
    return GeometryUtils::computeBarycentricCoord(p, v1, v2, v3);
}

vec3 Triangle::getBarycentricCoord(const vec3& p, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    return GeometryUtils::computeBarycentricCoord(p, v1, v2, v3);
}

bool Triangle::isWithinTriangle(const vec3& p) const noexcept {
    return GeometryUtils::isPointWithinTriangle(p, v1, v2, v3);
}

bool Triangle::isWithinTriangle(const vec3& p, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    return GeometryUtils::isPointWithinTriangle(p, v1, v2, v3);
}

vec3 Triangle::getClosestPtPointTriangle(const vec3& p) const noexcept {
    return GeometryUtils::getClosestPointBetweenPointAndTriangle(p, v1, v2, v3);
}

vec3 Triangle::getClosestPtPointTriangle(const vec3& p, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    return GeometryUtils::getClosestPointBetweenPointAndTriangle(p, v1, v2, v3);
}

bool Triangle::isSegmentIntersecting(vec3& c, const Line& line) const noexcept {
    return GeometryUtils::isSegmentIntersectingTriangle(c, line.getPointStart(), line.getPointEnd(), v1, v2, v3);
}

bool Triangle::isSegmentIntersecting(vec3& c, const vec3& start, const vec3& end) const noexcept {
    return GeometryUtils::isSegmentIntersectingTriangle(c, start, end, v1, v2, v3);
}

bool Triangle::isSegmentIntersecting(vec3& c, const Line& line, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    return GeometryUtils::isSegmentIntersectingTriangle(c, line.getPointStart(), line.getPointEnd(), v1, v2, v3);
}

bool Triangle::isSegmentIntersecting(vec3& c, const vec3& start, const vec3& end, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    return GeometryUtils::isSegmentIntersectingTriangle(c, start, end, v1, v2, v3);
}

GLfloat Triangle::getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const Line& line) const noexcept {
    return GeometryUtils::getClosestPointBetweenSegmentAndTriangle(c1, c2, line.getPointStart(), line.getPointEnd(), v1, v2, v3);
}

GLfloat Triangle::getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const vec3& start, const vec3& end) const noexcept {
    return GeometryUtils::getClosestPointBetweenSegmentAndTriangle(c1, c2, start, end, v1, v2, v3);
}

GLfloat Triangle::getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const Line& line, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    return GeometryUtils::getClosestPointBetweenSegmentAndTriangle(c1, c2, line.getPointStart(), line.getPointEnd(), v1, v2, v3);
}

GLfloat Triangle::getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const vec3& start, const vec3& end, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    return GeometryUtils::getClosestPointBetweenSegmentAndTriangle(c1, c2, start, end, v1, v2, v3);
}