#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <Camera.hpp>

class SceneGraph {
protected:
    shared_ptr<SceneObject> root = nullptr;
    shared_ptr<Camera> camera = nullptr;
    float fov = 45.f;
    float width = 512.f;
    float height = 512.f;
    float nearZ = 0.1f;
    float farZ = 100.f;

    shared_ptr<SceneObject> getSceneObject(const string& name, const shared_ptr<SceneObject>& sceneObject) const noexcept;

public:
    SceneGraph(const shared_ptr<SceneObject>& root = make_shared<SceneObject>(string("World_") + SceneUtils::createId()));

    virtual void draw(void) const noexcept;

    shared_ptr<SceneObject> getSceneObject(const string& name) const noexcept;

    shared_ptr<SceneObject>& getRoot(void) noexcept;

    const shared_ptr<Camera>& getCamera(void) const noexcept;

    const float& getFov(void) const noexcept;

    const float& getWidth(void) const noexcept;

    const float& getHeight(void) const noexcept;

    const float& getNearZ(void) const noexcept;

    const float& getFarZ(void) const noexcept;

    void setCamera(const shared_ptr<Camera>& camera) noexcept;

    void setFov(const float& fov) noexcept;

    void setWidth(const float& width) noexcept;

    void setHeight(const float& height) noexcept;

    void setNearZ(const float& nearZ) noexcept;

    void setFarZ(const float& farZ) noexcept;
};

#endif // !SCENE_GRAPH_HPP
