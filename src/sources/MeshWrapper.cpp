#include <MeshWrapper.hpp>

MeshWrapper::MeshWrapper(const shared_ptr<Mesh>& mesh, const string& name, const Transform& transform):
    SceneObject(name, transform),
    mesh(mesh) {
}

MeshWrapper::MeshWrapper(const MeshWrapper& meshWrapper):
    SceneObject(meshWrapper.name, meshWrapper.transform),
    mesh(meshWrapper.mesh) {
    children = meshWrapper.children;
}

MeshWrapper::MeshWrapper(MeshWrapper&& meshWrapper):
    SceneObject(SceneUtils::createId()) {
    children = move(meshWrapper.children);
    transform = move(meshWrapper.transform);
    mesh = move(meshWrapper.mesh);
}

MeshWrapper& MeshWrapper::operator=(const MeshWrapper& meshWrapper) noexcept {
    name = meshWrapper.name;
    transform = meshWrapper.transform;
    children = meshWrapper.children;
    mesh = meshWrapper.mesh;

    return *this;
}

MeshWrapper& MeshWrapper::operator=(MeshWrapper&& meshWrapper) noexcept {
    name = move(meshWrapper.name);
    transform = move(meshWrapper.transform);
    children = move(meshWrapper.children);
    mesh = move(meshWrapper.mesh);

    return *this;
}

void MeshWrapper::setMesh(const shared_ptr<Mesh>& mesh) noexcept {
    this->mesh = mesh;
}

shared_ptr<Mesh>& MeshWrapper::getMesh(void) noexcept {
    return mesh;
}

void MeshWrapper::draw(const mat4& ProjectionViewMatrix) const {
    if (mesh != nullptr) {
        mesh->draw(ProjectionViewMatrix, transform.getMatrix());
    }

    SceneObject::draw(ProjectionViewMatrix);
}