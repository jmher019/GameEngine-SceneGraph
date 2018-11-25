#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

// cpp
#include <vector>
#include <memory>
#include <string>

#include <Transform.hpp>
#include <EventManager.hpp>

using namespace std;
using namespace puggo;

class SceneObject {
protected:
    Transform transform = Transform();
    vector<shared_ptr<SceneObject>> children;
    string name = string("");
    EventManager manager;

public:
    struct ON_ADD_TO_SCENE_OBJECT {
        SceneObject const* parent;
        const shared_ptr<SceneObject>& child;
    };

    struct ON_REMOVE_FROM_SCENE_OBJECT {
        SceneObject const* parent;
        const shared_ptr<SceneObject>& child;
    };

public:
    SceneObject(const string& name, const Transform& transform = Transform());
    
    SceneObject(const SceneObject& sceneObject);

    SceneObject(SceneObject&& sceneObject);

    SceneObject& operator=(const SceneObject& other) noexcept;

    SceneObject& operator=(SceneObject&& other) noexcept;

    virtual ~SceneObject(void);

    virtual void draw(const mat4& ProjectionViewMatrix) const;

    const Transform& getTransform(void) const noexcept;

    void setTransform(const Transform& transform) noexcept;

    const string& getName(void) const noexcept;

    void setName(const string& name) noexcept;

    vector<shared_ptr<SceneObject>>& getChildren(void) noexcept;

    unsigned long subscribeToOnAddToSceneObject(function<void(const ON_ADD_TO_SCENE_OBJECT&)> callable) noexcept;

    unsigned long subscribeToOnRemoveFromSceneObject(function<void(const ON_REMOVE_FROM_SCENE_OBJECT&)> callable) noexcept;

    EventManager& getManager(void) noexcept;
};

#endif // !SCENE_OBJECT_HPP
