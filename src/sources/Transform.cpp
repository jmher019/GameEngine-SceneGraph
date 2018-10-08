#include <Transform.hpp>

Transform::Transform(const fdualquat& translationAndRotation, const vec3& scale):
    translationAndRotation(translationAndRotation),
    scale(scale) {
}

Transform::Transform(const Transform& transform):
    translationAndRotation(transform.translationAndRotation),
    scale(transform.scale) {
}

Transform::Transform(Transform&& transform):
    translationAndRotation(std::move(transform.translationAndRotation)),
    scale(std::move(transform.scale)) {
}

Transform Transform::operator*(const Transform& other) const noexcept {
    return Transform(translationAndRotation * other.translationAndRotation, scale * other.scale);
}

Transform& Transform::operator=(const Transform& other) noexcept {
    translationAndRotation = other.translationAndRotation;
    scale = other.scale;
    return *this;
}

Transform& Transform::operator=(Transform&& other) noexcept {
    translationAndRotation = std::move(other.translationAndRotation);
    scale = std::move(other.scale);
    return *this;
}

mat4x4 Transform::getMatrix(void) const noexcept {
    return mat4(transpose(mat3x4_cast(translationAndRotation))) * glm::scale(mat4x4(1.f), scale);
}

mat4x4 Transform::getInverseMatrix(void) const noexcept {
    return glm::scale(mat4x4(1.f), vec3(1.f / scale.x, 1.f / scale.y, 1.f / scale.z)) *
        mat4(transpose(mat3x4_cast(inverse(translationAndRotation))));
}

Transform Transform::getInverse(void) const noexcept {
    return inverse(*this);
}

vec3 Transform::getXUnitVector(void) const noexcept {
    return normalize(vec3(getMatrix() * vec4(1.f, 0.f, 0.f, 0.f)));
}

vec3 Transform::getYUnitVector(void) const noexcept {
    return normalize(vec3(getMatrix() * vec4(0.f, 1.f, 0.f, 0.f)));
}

vec3 Transform::getZUnitVector(void) const noexcept {
    return normalize(vec3(getMatrix() * vec4(0.f, 0.f, 1.f, 0.f)));
}

const fdualquat& Transform::getTranslationAndRotation(void) const noexcept {
    return translationAndRotation;
}

const vec3& Transform::getScale(void) const noexcept {
    return scale;
}

ostream& operator<< (ostream& out, const Transform& transform) noexcept {
    out << transform.getMatrix() << endl;
    return out;
}

ostream& operator<< (ostream& out, const mat4& mat) noexcept {
    out << std::fixed << std::setprecision(4) << endl;
    out << mat[0][0] << "\t\t" << mat[1][0] << "\t\t" << mat[2][0] << "\t\t" << mat[3][0] << endl;
    out << mat[0][1] << "\t\t" << mat[1][1] << "\t\t" << mat[2][1] << "\t\t" << mat[3][1] << endl;
    out << mat[0][2] << "\t\t" << mat[1][2] << "\t\t" << mat[2][2] << "\t\t" << mat[3][2] << endl;
    out << mat[0][3] << "\t\t" << mat[1][3] << "\t\t" << mat[2][3] << "\t\t" << mat[3][3] << endl;

    return out;
}

Transform inverse(const Transform& transform) noexcept {
    const vec3& scale = transform.getScale();
    return Transform(inverse(transform.getTranslationAndRotation()), vec3(1.f / scale.x, 1.f / scale.y, 1.f / scale.z));
}