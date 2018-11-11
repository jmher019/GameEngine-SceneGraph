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
    upVector(upVector),
    cameraMatrix(lookAt(eyePosition, lookAtPosition, upVector)) {
}

Camera::Camera(const Camera& camera):
    SceneObject(camera.name, camera.transform),
    eyePosition(camera.eyePosition),
    lookAtPosition(camera.lookAtPosition),
    upVector(camera.upVector),
    cameraMatrix(camera.cameraMatrix) {
}

Camera::Camera(Camera&& camera):
    SceneObject(std::move(camera.name), std::move(camera.transform)),
    eyePosition(std::move(camera.eyePosition)),
    lookAtPosition(std::move(camera.lookAtPosition)),
    upVector(std::move(camera.upVector)),
    cameraMatrix(std::move(camera.cameraMatrix)) {
}

Camera& Camera::operator=(const Camera& camera) noexcept {
    name = camera.name;
    transform = camera.transform;
    eyePosition = camera.eyePosition;
    lookAtPosition = camera.lookAtPosition;
    upVector = camera.upVector;
    cameraMatrix = camera.cameraMatrix;

    return *this;
}

Camera& Camera::operator=(Camera&& camera) noexcept {
    name = std::move(camera.name);
    transform = std::move(camera.transform);
    eyePosition = std::move(camera.eyePosition);
    lookAtPosition = std::move(camera.lookAtPosition);
    upVector = std::move(camera.upVector);
    cameraMatrix = std::move(camera.cameraMatrix);

    return *this;
}

void Camera::updateCameraMatrix(void) noexcept {
    const mat4 matrix = transform.getMatrix();
    cameraMatrix = lookAt(
        vec3(matrix * vec4(eyePosition, 1.f)),
        vec3(matrix * vec4(lookAtPosition, 1.f)),
        upVector
    );
}

void Camera::translate(const float& tX, const float& tY, const float& tZ) noexcept {
    SceneObject::translate(tX, tY, tZ);
    updateCameraMatrix();
}

void Camera::rotate(const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    SceneObject::rotate(degreesX, degreesY, degreesZ);
    updateCameraMatrix();
}

void Camera::orbit(const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    SceneObject::orbit(degreesX, degreesY, degreesZ);
    updateCameraMatrix();
}

void Camera::resize(const float& sX, const float& sY, const float& sZ) noexcept {
    SceneObject::resize(sX, sY, sZ);
    updateCameraMatrix();
}

const vec3& Camera::getEyePosition(void) const noexcept {
    return eyePosition;
}

Camera& Camera::setEyePosition(const vec3& eyePosition) noexcept {
    this->eyePosition = eyePosition;
    updateCameraMatrix();
    return *this;
}

const vec3& Camera::getLookAtPosition(void) const noexcept {
    return lookAtPosition;
}

Camera& Camera::setLookAtPosition(const vec3& lookAtPosition) noexcept {
    this->lookAtPosition = lookAtPosition;
    updateCameraMatrix();
    return *this;
}

const vec3& Camera::getUpVector(void) const noexcept {
    return upVector;
}

Camera& Camera::setUpVector(const vec3& upVector) noexcept {
    this->upVector = upVector;
    updateCameraMatrix();
    return *this;
}

const mat4& Camera::getCameraMatrix(void) const noexcept {
    return cameraMatrix;
}