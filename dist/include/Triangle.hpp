#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <vector>

#include <Line.hpp>
#include <Plane.hpp>

class Triangle {
private:
    vec3 v1;
    vec3 v2;
    vec3 v3;

public:
    Triangle(const vec3& v1, const vec3& v2, const vec3& v3);

    Triangle(const Triangle& triangle);

    Triangle(Triangle&& triangle);

    Triangle& operator=(const Triangle& triangle) noexcept;

    Triangle& operator=(Triangle&& triangle) noexcept;

    const vec3& getVertex1(void) const noexcept;

    void setVertex1(const vec3& v1) noexcept;

    const vec3& getVertex2(void) const noexcept;

    void setVertex2(const vec3& v2) noexcept;

    const vec3& getVertex3(void) const noexcept;

    void setVertex3(const vec3& v3) noexcept;

    vec3 getNormal(void) const noexcept;

    static vec3 getNormal(const vec3& v1, const vec3& v2, const vec3& v3) noexcept;

    vec3 getBarycentricCoord(const vec3& p) const noexcept;

    static vec3 getBarycentricCoord(const vec3& p, const vec3& v1, const vec3& v2, const vec3& v3) noexcept;

    bool isWithinTriangle(const vec3& p) const noexcept;

    static bool isWithinTriangle(const vec3& p, const vec3& v1, const vec3& v2, const vec3& v3) noexcept;

    vec3 getClosestPtPointTriangle(const vec3& p) const noexcept;

    static vec3 getClosestPtPointTriangle(const vec3& p, const vec3& v1, const vec3& v2, const vec3& v3) noexcept;

    bool isSegmentIntersecting(vec3& c, const Line& line) const noexcept;

    bool isSegmentIntersecting(vec3& c, const vec3& start, const vec3& end) const noexcept;

    static bool isSegmentIntersecting(vec3& c, const Line& line, const vec3& v1, const vec3& v2, const vec3& v3) noexcept;

    static bool isSegmentIntersecting(vec3& c, const vec3& start, const vec3& end, const vec3& v1, const vec3& v2, const vec3& v3) noexcept;

    GLfloat getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const Line& line) const noexcept;

    GLfloat getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const vec3& start, const vec3& end) const noexcept;

    static GLfloat getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const Line& line, const vec3& v1, const vec3& v2, const vec3& v3) noexcept;

    static GLfloat getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const vec3& start, const vec3& end, const vec3& v1, const vec3& v2, const vec3& v3) noexcept;
};

#endif // !TRIANGLE_HPP
