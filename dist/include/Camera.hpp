#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SceneObject.hpp>

class Camera : public SceneObject {
private:
    vec3 eyePosition = vec3(0.f, 0.f, 5.f);
    vec3 lookAtPosition = vec3(0.f, 0.f, 0.f);
    vec3 upVector = vec3(0.f, 1.f, 0.f);
    mat4 cameraMatrix = lookAt(eyePosition, lookAtPosition, upVector);

    void updateCameraMatrix(void) noexcept;

public:
    Camera(
        const vec3& eyePosition = vec3(0.f, 0.f, 5.f),
        const vec3& lookAtPosition = vec3(0.f, 0.f, 0.f),
        const vec3& upVector = vec3(0.f, 1.f, 0.f),
        const string& name = string(""),
        const Transform& transform = Transform()
    );

    Camera(const Camera& camera);

    Camera(Camera&& camera);

    Camera& operator=(const Camera& camera) noexcept;

    Camera& operator=(Camera&& camera) noexcept;

    void update(const Transform& newTransform) override;

    const vec3& getEyePosition(void) const noexcept;

    Camera& setEyePosition(const vec3& eyePosition) noexcept;

    const vec3& getLookAtPosition(void) const noexcept;

    Camera& setLookAtPosition(const vec3& lookAtPosition) noexcept;

    const vec3& getUpVector(void) const noexcept;

    Camera& setUpVector(const vec3& upVector) noexcept;

    const mat4& getCameraMatrix(void) const noexcept;
};

#endif // !CAMERA_HPP
