#include <CameraFactory.hpp>

shared_ptr<Camera> CameraFactory::playerCamera = nullptr;
vector<shared_ptr<Camera>> CameraFactory::cameras;

shared_ptr<Camera> CameraFactory::createCamera(
    const vec3& eyePosition,
    const vec3& lookAtPosition,
    const vec3& upVector
) noexcept {
    return make_shared<Camera>(eyePosition, lookAtPosition, upVector);
}

void CameraFactory::setPlayerCamera(const shared_ptr<Camera>& camera) noexcept {
    playerCamera = camera;
}

const shared_ptr<Camera>& CameraFactory::getPlayerCamera(void) noexcept {
    return playerCamera;
}

void CameraFactory::loadPlayerCameraIntoShader(const shared_ptr<Shader>& shader) noexcept {
    if (playerCamera != nullptr) {
        shader->setVec3("viewPos", value_ptr(playerCamera->getEyePosition()));
    }
}