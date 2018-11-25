#include <Camera.hpp>

Camera::Camera(
    const vec3& eyePosition,
    const vec3& lookAtPosition,
    const vec3& upVector,
    const string& name,
    const Transform& transform
): 
    SceneObject(name, transform),
    eyePosition(eyePosition),
    lookAtPosition(lookAtPosition),
    upVector(upVector) {
}

Camera::Camera(const Camera& camera):
    SceneObject(camera.name, camera.transform),
    eyePosition(camera.eyePosition),
    lookAtPosition(camera.lookAtPosition),
    upVector(camera.upVector) {
}

Camera::Camera(Camera&& camera):
    SceneObject(std::move(camera.name), std::move(camera.transform)),
    eyePosition(std::move(camera.eyePosition)),
    lookAtPosition(std::move(camera.lookAtPosition)),
    upVector(std::move(camera.upVector)) {
}

Camera& Camera::operator=(const Camera& camera) noexcept {
    name = camera.name;
    transform = camera.transform;
    eyePosition = camera.eyePosition;
    lookAtPosition = camera.lookAtPosition;
    upVector = camera.upVector;

    return *this;
}

Camera& Camera::operator=(Camera&& camera) noexcept {
    name = std::move(camera.name);
    transform = std::move(camera.transform);
    eyePosition = std::move(camera.eyePosition);
    lookAtPosition = std::move(camera.lookAtPosition);
    upVector = std::move(camera.upVector);

    return *this;
}

const vec3& Camera::getEyePosition(void) const noexcept {
    return eyePosition;
}

Camera& Camera::setEyePosition(const vec3& eyePosition) noexcept {
    this->eyePosition = eyePosition;
    return *this;
}

const vec3& Camera::getLookAtPosition(void) const noexcept {
    return lookAtPosition;
}

Camera& Camera::setLookAtPosition(const vec3& lookAtPosition) noexcept {
    this->lookAtPosition = lookAtPosition;
    return *this;
}

const vec3& Camera::getUpVector(void) const noexcept {
    return upVector;
}

Camera& Camera::setUpVector(const vec3& upVector) noexcept {
    this->upVector = upVector;
    return *this;
}

const mat4& Camera::getCameraMatrix(void) const noexcept {
    const mat4 matrix = transform.getMatrix();
    return lookAt(
        vec3(matrix * vec4(eyePosition, 1.f)),
        vec3(matrix * vec4(lookAtPosition, 1.f)),
        upVector
    );
}