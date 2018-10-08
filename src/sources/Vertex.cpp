#include <Vertex.hpp>

Vertex::Vertex(const vec3& position, const vec3& normal, const vec3& color, const vec2& texCoord):
    position(position),
    normal(normal),
    color(color),
    texCoord(texCoord) {
}

Vertex::Vertex(const Vertex& vertex):
    position(vertex.position),
    normal(vertex.normal),
    color(vertex.color),
    texCoord(vertex.texCoord) {
}

Vertex::Vertex(Vertex&& vertex):
    position(std::move(vertex.position)),
    normal(std::move(vertex.normal)),
    color(std::move(vertex.color)),
    texCoord(std::move(vertex.texCoord)) {
}

Vertex& Vertex::operator=(const Vertex& other) noexcept {
    position = other.position;
    normal = other.normal;
    color = other.color;
    texCoord = other.texCoord;
    return *this;
}

Vertex& Vertex::operator=(Vertex&& other) noexcept {
    position = std::move(other.position);
    normal = std::move(other.normal);
    color = std::move(other.color);
    texCoord = std::move(other.texCoord);
    return *this;
}

ostream& operator << (ostream& out, const vec2& vector) {
    out << std::fixed << std::setprecision(4) << endl;
    out << vector[0] << "\t\t" << vector[1] << endl;
    return out;
}

ostream& operator << (ostream& out, const vec3& vector) {
    out << std::fixed << std::setprecision(4) << endl;
    out << vector[0] << "\t\t" << vector[1] << "\t\t" << vector[2] << endl;
    return out;
}

ostream& operator<< (ostream& out, const Vertex& vertex) {
    out << "Vertex position: " << vertex.position << endl;
    out << "Vertex normal: " << vertex.normal << endl;
    out << "Vertex color: " << vertex.color << endl;
    out << "Vertex texCoord: " << vertex.texCoord << endl;

    return out;
}