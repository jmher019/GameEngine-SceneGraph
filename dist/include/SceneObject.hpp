#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

// cpp
#include <vector>
#include <memory>
#include <string>

#include <Transform.hpp>

using namespace std;

class SceneObject {
protected:
    Transform transform = Transform();
    vector<shared_ptr<SceneObject>> children;
    SceneObject* parent;
    string name = string("");

public:
    SceneObject(const string& name = string(""), const Transform& transform = Transform());
    
    SceneObject(const SceneObject& sceneObject);

    SceneObject(SceneObject&& sceneObject);

    SceneObject& operator=(const SceneObject& other) noexcept;

    SceneObject& operator=(SceneObject&& other) noexcept;

    virtual ~SceneObject(void);

    virtual void draw(const mat4& ProjectionViewMatrix) const;

    virtual void translate(const float& tX, const float& tY, const float& tZ) noexcept;

    virtual void rotate(const float& degreesX, const float& degreesY, const float& degreesZ) noexcept;

    virtual void orbit(const float& degreesX, const float& degreesY, const float& degreesZ) noexcept;

    virtual void resize(const float& sX, const float& sY, const float& sZ) noexcept;

    const Transform& getTransform(void) const noexcept;

    const string& getName(void) const noexcept;

    void setName(const string& name) noexcept;

    const vector<shared_ptr<SceneObject>>& getChildren(void) const noexcept;

    SceneObject * const & getParent(void) noexcept;

    void appendChild(const shared_ptr<SceneObject>& child) noexcept;

    bool replaceChild(const shared_ptr<SceneObject>& existingChild, const shared_ptr<SceneObject>& newChild) noexcept;

    bool removeChild(const shared_ptr<SceneObject>& child) noexcept;
};

ostream& operator<< (ostream& out, const SceneObject& sceneObject);

#endif // !SCENE_OBJECT_HPP
