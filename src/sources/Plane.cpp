#include <Plane.hpp>

#include <Plane.hpp>

Plane::Plane(const vec3& p1, const vec3& p2, const vec3& p3) {
    n = cross(p2 - p1, p3 - p1);
    d = dot(n, p1);
}

Plane::Plane(const vec3& n, const GLfloat& d):
    n(n),
    d(d){
}

Plane::Plane(const Plane& plane):
    n(plane.n),
    d(plane.d){
}

Plane::Plane(Plane&& plane):
    n(move(plane.n)),
    d(move(plane.d)) {
}

Plane& Plane::operator=(const Plane& plane) noexcept {
    n = plane.n;
    d = plane.d;

    return *this;
}

Plane& Plane::operator=(Plane&& plane) noexcept {
    n = move(plane.n);
    d = move(plane.d);

    return *this;
}

vec3 Plane::closestPtPointPlane(const vec3& p) const noexcept {
    return GeometryUtils::getClosestPointBetweenPointAndPlane(p, n, d);
}

vec3 Plane::closestPtPointPlane(const vec3& p, const vec3& n, const GLfloat& d) noexcept {
    return GeometryUtils::getClosestPointBetweenPointAndPlane(p, n, d);
}

bool Plane::isPointOutsideOfPlane(const vec3& p) const noexcept {
    return GeometryUtils::isPointOutsideOfPlane(p, n, d);
}

bool Plane::isPointOutsideOfPlane(const vec3& p, const vec3& n, const GLfloat& d) noexcept {
    return GeometryUtils::isPointOutsideOfPlane(p, n, d);
}

bool Plane::arePointsOnOppositeSides(const vec3& p1, const vec3& p2) const noexcept {
    return GeometryUtils::arePointsOnOppositeSidesOfPlane(p1, p2, n, d);
}

bool Plane::arePointsOnOppositeSides(const vec3& p1, const vec3& p2, const vec3& n, const GLfloat& d) noexcept {
    return GeometryUtils::arePointsOnOppositeSidesOfPlane(p1, p2, n, d);
}