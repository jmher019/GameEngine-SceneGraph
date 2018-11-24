#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <SceneObject.hpp>

class SceneGraph {
protected:
    shared_ptr<SceneObject> root = nullptr;

    shared_ptr<SceneObject> getSceneObject(const string& name, const shared_ptr<SceneObject>& sceneObject) const noexcept;

public:
    SceneGraph(const shared_ptr<SceneObject>& root = make_shared<SceneObject>(string("World")));

    virtual void draw(const mat4& ProjectionViewMatrix) const noexcept;

    shared_ptr<SceneObject> getSceneObject(const string& name) const noexcept;

    const shared_ptr<SceneObject>& getRoot(void) const noexcept;
};

ostream& operator<< (ostream& out, const SceneGraph& sceneGraph);

#endif // !SCENE_GRAPH_HPP
