#ifndef CAMERA_FACTORY_HPP
#define CAMERA_FACTORY_HPP

#include <Camera.hpp>
#include <Shader.hpp>

#include <glm/gtc/type_ptr.hpp>

class CameraFactory {
private:
    static shared_ptr<Camera> playerCamera;
    static vector<shared_ptr<Camera>> cameras;

public:
    static shared_ptr<Camera> createCamera(
        const vec3& eyePosition,
        const vec3& lookAtPosition,
        const vec3& upVector
    ) noexcept;

    static void setPlayerCamera(const shared_ptr<Camera>& camera) noexcept;

    static const shared_ptr<Camera>& getPlayerCamera(void) noexcept;

    static void loadPlayerCameraIntoShader(const shared_ptr<Shader>& shader) noexcept;
};

#endif // !CAMERA_FACTORY_HPP
