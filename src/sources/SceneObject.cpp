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

void SceneObject::translate(const float& tX, const float& tY, const float& tZ) noexcept {
    Transform newTransform(
        fdualquat(fquat(1.f, 0.f, 0.f, 0.f), vec3(tX, tY, tZ))
    );

    transform = newTransform * transform;

    for (auto& child : children) {
        child->translate(tX, tY, tZ);
    }
}

void SceneObject::rotate(const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    vec3 translation = vec3(transform.getMatrix()[3]);
    Transform newTransform(
        fdualquat(fquat(1.f, 0.f, 0.f, 0.f), translation) *
        fdualquat(glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degreesX), vec3(1.f, 0.f, 0.f))) *
        fdualquat(glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degreesY), vec3(0.f, 1.f, 0.f))) *
        fdualquat(glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degreesZ), vec3(0.f, 0.f, 1.f))) *
        fdualquat(fquat(1.f, 0.f, 0.f, 0.f), -translation)
    );

    transform = newTransform * transform;

    for (auto& child : children) {
        child->rotate(degreesX, degreesY, degreesZ);
    }
}

void SceneObject::orbit(const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    Transform newTransform(
        fdualquat(glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degreesX), vec3(1.f, 0.f, 0.f))) *
        fdualquat(glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degreesY), vec3(0.f, 1.f, 0.f))) *
        fdualquat(glm::rotate(fquat(1.f, 0.f, 0.f, 0.f), radians(degreesZ), vec3(0.f, 0.f, 1.f)))
    );

    transform = newTransform * transform;

    for (auto& child : children) {
        child->orbit(degreesX, degreesY, degreesZ);
    }
}

void SceneObject::resize(const float& sX, const float& sY, const float& sZ) noexcept {
    Transform newTransform(
        fdualquat(fquat(1.f, 0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f)),
        vec3(sX, sY, sZ)
    );

    transform = newTransform * transform;

    for (auto& child : children) {
        child->resize(sX, sY, sZ);
    }
}

const Transform& SceneObject::getTransform(void) const noexcept {
    return transform;
}

const string& SceneObject::getName(void) const noexcept {
    return name;
}

void SceneObject::setName(const string& name) noexcept {
    this->name = name;
}

const vector<shared_ptr<SceneObject>>& SceneObject::getChildren(void) const noexcept {
    return children;
}

void SceneObject::appendChild(const shared_ptr<SceneObject>& child) noexcept {
    children.push_back(child);
}

bool SceneObject::removeChild(const shared_ptr<SceneObject>& child) noexcept {
    auto it = children.begin();
    for (; it != children.end(); it++) {
        if ((*it) == child) {
            children.erase(it);
            return true;
        }
    }

    return false;
}

ostream& operator<< (ostream& out, const SceneObject& sceneObject) {
    out << "Scene Object name: " << sceneObject.getName() << endl;
    out << "Scene Object transform:\n" << sceneObject.getTransform() << endl;
    
    if (!sceneObject.getChildren().empty()) {
        out << "Scene Object children:\n" << endl;
        for (auto& child : sceneObject.getChildren()) {
            out << *child << endl;
        }
    }

    return out;
}