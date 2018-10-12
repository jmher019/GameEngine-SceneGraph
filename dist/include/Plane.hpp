#ifndef PLANE_HPP
#define PLANE_HPP

#include <Vertex.hpp>

class Plane {
private:
    vec3 n;
    GLfloat d;

public:
    Plane(const vec3& p1, const vec3& p2, const vec3& p3);

    Plane(const vec3& n, const GLfloat& d);

    Plane(const Plane& plane);

    Plane(Plane&& plane);

    Plane& operator=(const Plane& plane) noexcept;

    Plane& operator=(Plane&& plane) noexcept;

    vec3 closestPtPointPlane(const vec3& p) const noexcept;

    static vec3 closestPtPointPlane(const vec3& p, const vec3& n, const GLfloat& d) noexcept;

    bool isPointOutsideOfPlane(const vec3& p) const noexcept;

    static bool isPointOutsideOfPlane(const vec3&p, const vec3& n, const GLfloat& d) noexcept;

    bool arePointsOnOppositeSides(const vec3& p1, const vec3& p2) const noexcept;

    static bool arePointsOnOppositeSides(const vec3& p1, const vec3& p2, const vec3& n, const GLfloat& d) noexcept;
};

#endif // !PLANE_HPP
