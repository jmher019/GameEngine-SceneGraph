#include <Rigidbody.hpp>

Rigidbody::Rigidbody(
    const shared_ptr<MeshWrapper> meshWrapper,
    const shared_ptr<BoundingVolume> boundingVolume,
    const string& name,
    const Transform& transform
):
    SceneObject(name, transform),
    meshWrapper(meshWrapper),
    boundingVolume(boundingVolume) {

    if (meshWrapper == nullptr) {
        throw invalid_argument("meshWrapper must not be a nullptr");
    }

    if (boundingVolume == nullptr) {
        throw invalid_argument("boundingVolume must not be a nullptr");
    }

    // remove all children of boundingVolume
    while (!boundingVolume->getChildren().empty()) {
        boundingVolume->removeChild(boundingVolume->getChildren()[0]);
    }

    boundingVolume->appendChild(meshWrapper);
    appendChild(boundingVolume);
}

Rigidbody::Rigidbody(const Rigidbody& rigidbody):
    SceneObject(rigidbody.name, rigidbody.transform),
    meshWrapper(rigidbody.meshWrapper),
    boundingVolume(rigidbody.boundingVolume) {
    children = rigidbody.children;
}

Rigidbody::Rigidbody(Rigidbody&& rigidbody) {
    name = move(rigidbody.name);
    transform = move(rigidbody.transform);
    meshWrapper = move(rigidbody.meshWrapper);
    boundingVolume = move(rigidbody.boundingVolume);
    children = move(rigidbody.children);
}

Rigidbody& Rigidbody::operator=(const Rigidbody& rigidbody) noexcept {
    name = rigidbody.name;
    transform = rigidbody.transform;
    meshWrapper = rigidbody.meshWrapper;
    boundingVolume = rigidbody.boundingVolume;
    children = rigidbody.children;

    return *this;
}

Rigidbody& Rigidbody::operator=(Rigidbody&& rigidbody) noexcept {
    name = move(rigidbody.name);
    transform = move(rigidbody.transform);
    meshWrapper = move(rigidbody.meshWrapper);
    boundingVolume = move(rigidbody.boundingVolume);
    children = move(rigidbody.children);

    return *this;
}

void Rigidbody::setMeshWrapper(const shared_ptr<MeshWrapper>& meshWrapper) {
    if (meshWrapper == nullptr) {
        throw invalid_argument("meshWrapper must not be a nullptr");
    }

    boundingVolume->replaceChild(this->meshWrapper, meshWrapper);
    this->meshWrapper = meshWrapper;
}

shared_ptr<MeshWrapper>& Rigidbody::getMeshWrapper(void) noexcept {
    return meshWrapper;
}

void Rigidbody::setBoundingVolume(const shared_ptr<BoundingVolume>& boundingVolume) {
    if (boundingVolume == nullptr) {
        throw invalid_argument("boundingVolume must not be a nullptr");
    }

    replaceChild(this->boundingVolume, boundingVolume);
    this->boundingVolume = boundingVolume;
}

shared_ptr<BoundingVolume>& Rigidbody::getBoundingVolume(void) noexcept {
    return boundingVolume;
}