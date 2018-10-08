#include <SceneGraph.hpp>

SceneGraph::SceneGraph(const shared_ptr<SceneObject>& root):
    root(root) {
}

void SceneGraph::draw(const mat4& ProjectionViewMatrix) const noexcept {
    root->draw(ProjectionViewMatrix);
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

ostream& operator<< (ostream& out, const SceneGraph& sceneGraph) {
    out << "Scene Graph:\nRoot node:\n";

    if (sceneGraph.getRoot() != nullptr) {
        out << *sceneGraph.getRoot() << endl;
    } else {
        out << "None" << endl;
    }

    return out;
}