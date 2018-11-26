#include <SceneObject.hpp>

SceneObject::SceneObject(const string& name, const Transform& transform):
    name(name),
    transform(transform) {
}

SceneObject::SceneObject(const SceneObject& sceneObject):
    name(sceneObject.name),
    transform(sceneObject.transform),
    children(sceneObject.children) {
}

SceneObject::SceneObject(SceneObject&& sceneObject):
    name(std::move(sceneObject.name)),
    transform(std::move(sceneObject.transform)),
    children(std::move(sceneObject.children)) {
}

SceneObject::~SceneObject(void) {
    children.clear();
}

SceneObject& SceneObject::operator=(const SceneObject& other) noexcept {
    name = other.name;
    transform = other.transform;
    children = other.children;
    return *this;
}

SceneObject& SceneObject::operator=(SceneObject&& other) noexcept {
    name = std::move(other.name);
    transform = std::move(other.transform);
    children = std::move(other.children);
    return *this;
}

void SceneObject::draw(const mat4& ProjectionViewMatrix) const {
    for (auto& child : children) {
        child->draw(ProjectionViewMatrix);
    }
}

const Transform& SceneObject::getTransform(void) const noexcept {
    return transform;
}

void SceneObject::setTransform(const Transform& transform) noexcept {
    this->transform = transform;
}

const string& SceneObject::getName(void) const noexcept {
    return name;
}

void SceneObject::setName(const string& name) noexcept {
    this->name = name;
}

vector<shared_ptr<SceneObject>>& SceneObject::getChildren(void) noexcept {
    return children;
}

unsigned long SceneObject::subscribeToOnAddToSceneObject(function<void(const ON_ADD_TO_SCENE_OBJECT&)> callable) noexcept {
    return manager.subscribe(callable);
}

unsigned long SceneObject::subscribeToOnRemoveFromSceneObject(function<void(const ON_REMOVE_FROM_SCENE_OBJECT&)> callable) noexcept {
    return manager.subscribe(callable);
}

unsigned long SceneObject::subscribeToOnChildAppended(function<void(const ON_CHILD_APPENDED&)> callable) noexcept {
    return manager.subscribe(callable);
}

EventManager& SceneObject::getManager(void) noexcept {
    return manager;
}