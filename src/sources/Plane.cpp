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
    const float t = (dot(n, p) - d) / dot(n, n);
    return p - t * n;
}

vec3 Plane::closestPtPointPlane(const vec3& p, const vec3& n, const GLfloat& d) noexcept {
    const float t = (dot(n, p) - d) / dot(n, n);
    return p - t * n;
}

bool Plane::isPointOutsideOfPlane(const vec3& p) const noexcept {
    return fabs(dot(p, n) - d) > GeometryUtils::epsilon;
}

bool Plane::isPointOutsideOfPlane(const vec3& p, const vec3& n, const GLfloat& d) noexcept {
    return fabs(dot(p, n) - d) > GeometryUtils::epsilon;
}

bool Plane::arePointsOnOppositeSides(const vec3& p1, const vec3& p2) const noexcept {
    const float signp1 = dot(vec4(p1, 1.f), vec4(n, d));
    const float signp2 = dot(vec4(p2, 1.f), vec4(n, d));

    return signp1 * signp2 < 0.f;
}

bool Plane::arePointsOnOppositeSides(const vec3& p1, const vec3& p2, const vec3& n, const GLfloat& d) noexcept {
    const float signp1 = dot(vec4(p1, 1.f), vec4(n, d));
    const float signp2 = dot(vec4(p2, 1.f), vec4(n, d));

    return signp1 * signp2 < 0.f;
}