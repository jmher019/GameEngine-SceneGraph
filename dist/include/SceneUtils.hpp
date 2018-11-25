#ifndef SCENE_UTILS_HPP
#define SCENE_UTILS_HPP

#include <SceneObject.hpp>

#include <algorithm>

class SceneUtils {
private:
    static unsigned long counter;

public:
    SceneUtils() = delete;
    SceneUtils(const SceneUtils& sceneUtils) = delete;
    SceneUtils(SceneUtils&& sceneUtils) = delete;
    SceneObject& operator=(const SceneUtils& sceneUtils) = delete;
    SceneObject& operator=(SceneUtils&& sceneUtils) = delete;

    static void translate(SceneObject& sceneObject, const vec3& t) noexcept;
    static void translate(SceneObject& sceneObject, const float& tX, const float& tY, const float& tZ) noexcept;

    static void rotate(SceneObject& sceneObject, const vec3& degrees) noexcept;
    static void rotate(SceneObject& sceneObject, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept;

    static void orbit(SceneObject& sceneObject, const vec3& degrees) noexcept;
    static void orbit(SceneObject& sceneObject, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept;

    static void scale(SceneObject& sceneObject, const vec3& s) noexcept;
    static void scale(SceneObject& sceneObject, const float& sX, const float& sY, const float& sZ) noexcept;

    static void appendChild(SceneObject& sceneObject, const shared_ptr<SceneObject>& newChild) noexcept;

    static void insertChild(SceneObject& sceneObject, const size_t& index, const shared_ptr<SceneObject>& newChild) noexcept;

    static void removeChild(SceneObject& sceneObject, const shared_ptr<SceneObject>& newChild) noexcept;
    static void removeChild(SceneObject& sceneObject, const size_t& index) noexcept;

    static void replaceChild(SceneObject& sceneObject, const size_t& index, const shared_ptr<SceneObject>& newChild) noexcept;

    static long findChildIndex(SceneObject& sceneObject, const shared_ptr<SceneObject>& child) noexcept;

    static string createId(void) noexcept;
};

#endif // !SCENE_UTILS_HPP
