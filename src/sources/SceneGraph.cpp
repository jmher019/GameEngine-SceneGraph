#include <SceneGraph.hpp>

SceneGraph::SceneGraph(const shared_ptr<SceneObject>& root):
    root(root) {
}

void SceneGraph::draw() const noexcept {
    if (camera != nullptr) {
        root->draw(perspectiveFov(fov, width, height, nearZ, farZ) * camera->getCameraMatrix());
    }
}

shared_ptr<SceneObject> SceneGraph::getSceneObject(const string& name) const noexcept {
    return getSceneObject(name, root);
}

shared_ptr<SceneObject> SceneGraph::getSceneObject(const string& name, const shared_ptr<SceneObject>& sceneObject) const noexcept {
    if (sceneObject->getName() == name) {
        return sceneObject;
    }

    for (size_t i = 0; i < sceneObject->getChildren().size(); i++) {
        shared_ptr<SceneObject> foundChild = getSceneObject(name, sceneObject->getChildren()[i]);

        if (foundChild != nullptr) {
            return foundChild;
        }
    }

    return nullptr;
}

const shared_ptr<SceneObject>& SceneGraph::getRoot(void) const noexcept {
    return root;
}

const shared_ptr<Camera>& SceneGraph::getCamera(void) const noexcept {
    return camera;
}

const float& SceneGraph::getFov(void) const noexcept {
    return fov;
}

const float& SceneGraph::getWidth(void) const noexcept {
    return width;
}

const float& SceneGraph::getHeight(void) const noexcept {
    return height;
}

const float& SceneGraph::getNearZ(void) const noexcept {
    return nearZ;
}

const float& SceneGraph::getFarZ(void) const noexcept {
    return farZ;
}

void SceneGraph::setCamera(const shared_ptr<Camera>& camera) noexcept {
    this->camera = camera;
}

void SceneGraph::setFov(const float& fov) noexcept {
    this->fov = fov;
}

void SceneGraph::setWidth(const float& width) noexcept {
    this->width = width;
}

void SceneGraph::setHeight(const float& height) noexcept {
    this->height = height;
}

void SceneGraph::setNearZ(const float& nearZ) noexcept {
    this->nearZ = nearZ;
}

void SceneGraph::setFarZ(const float& farZ) noexcept {
    this->farZ = farZ;
}