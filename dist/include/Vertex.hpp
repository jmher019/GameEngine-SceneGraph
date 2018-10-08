#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <iomanip>

#include <glad\glad.h>
#include <glm\glm.hpp>

using namespace std;
using namespace glm;

class Vertex {
public:
    Vertex(
        const vec3& position = vec3(0.f, 0.f, 0.f),
        const vec3& normal = vec3(0.f, 0.f, 0.f),
        const vec3& color = vec3(0.f, 0.f, 0.f),
        const vec2& texCoord = vec2(0.f, 0.f)
    );

    Vertex(const Vertex& vertex);

    Vertex(Vertex&& vertex);

    Vertex& operator=(const Vertex& other) noexcept;

    Vertex& operator=(Vertex&& other) noexcept;

    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
};

ostream& operator<< (ostream& out, const vec2& vector);

ostream& operator<< (ostream& out, const vec3& vector);

ostream& operator<< (ostream& out, const Vertex& vertex);

#endif // !VERTEX_HPP
