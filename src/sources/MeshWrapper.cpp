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

MeshWrapper::MeshWrapper(MeshWrapper&& meshWrapper) {
    name = move(meshWrapper.name);
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

void MeshWrapper::setMeshWrapper(const shared_ptr<Mesh>& mesh) noexcept {
    this->mesh = mesh;
}

const shared_ptr<Mesh>& MeshWrapper::getMeshWrapper(void) const noexcept {
    return mesh;
}

void MeshWrapper::draw(const mat4& ProjectionViewMatrix) const {
    if (mesh != nullptr) {
        mesh->draw(ProjectionViewMatrix);
    }

    SceneObject::draw(ProjectionViewMatrix);
}