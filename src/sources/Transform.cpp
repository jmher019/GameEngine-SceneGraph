#include <Transform.hpp>

using namespace puggo;

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

Transform puggo::inverse(const Transform& transform) noexcept {
    const vec3& scale = transform.getScale();
    return Transform(inverse(transform.getTranslationAndRotation()), vec3(1.f / scale.x, 1.f / scale.y, 1.f / scale.z));
}

Transform puggo::translate(const Transform& transform, const vec3& t) noexcept {
    return Transform(
        fdualquat(fquat(1.f, 0.f, 0.f, 0.f), t) * transform.getTranslationAndRotation(),
        transform.getScale()
    );
}

Transform puggo::translate(const Transform& transform, const float& tX, const float& tY, const float& tZ) noexcept {
    return translate(transform, vec3(tX, tY, tZ));
}

Transform puggo::rotate(const Transform& transform, const vec3& degrees) noexcept {
    const fdualquat dualQuat = transform.getTranslationAndRotation();
    const fquat rotX = glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degrees.x), vec3(1.f, 0.f, 0.f));
    const fquat rotY = glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degrees.y), vec3(0.f, 1.f, 0.f));
    const fquat rotZ = glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degrees.z), vec3(0.f, 0.f, 1.f));
    return Transform(
        fdualquat(rotZ * rotY * rotX * dualQuat.real, dualQuat.dual),
        transform.getScale()
    );
}

Transform puggo::rotate(const Transform& transform, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    return rotate(transform, vec3(degreesX, degreesY, degreesZ));
}

Transform puggo::orbit(const Transform& transform, const vec3& degrees) noexcept {
    const fdualquat rotX = fdualquat(glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degrees.x), vec3(1.f, 0.f, 0.f)), vec3(0.f, 0.f, 0.f));
    const fdualquat rotY = fdualquat(glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degrees.y), vec3(0.f, 1.f, 0.f)), vec3(0.f, 0.f, 0.f));
    const fdualquat rotZ = fdualquat(glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degrees.z), vec3(0.f, 0.f, 1.f)), vec3(0.f, 0.f, 0.f));
    return Transform(
        rotZ * rotY * rotX * transform.getTranslationAndRotation(),
        transform.getScale()
    );
}

Transform puggo::orbit(const Transform& transform, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    return orbit(transform, vec3(degreesX, degreesY, degreesZ));
}

Transform puggo::scale(const Transform& transform, const vec3& s) noexcept {
    return Transform(
        transform.getTranslationAndRotation(),
        s * transform.getScale()
    );
}

Transform puggo::scale(const Transform& transform, const float& sX, const float& sY, const float& sZ) noexcept {
    return scale(transform, vec3(sX, sY, sZ));
}