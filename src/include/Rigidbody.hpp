#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <MeshWrapper.hpp>
#include <BoundingVolume.hpp>

#include <stdexcept>

class Rigidbody : public SceneObject {
private:
    shared_ptr<MeshWrapper> meshWrapper;
    shared_ptr<BoundingVolume> boundingVolume;

public:
    Rigidbody(
        const shared_ptr<MeshWrapper> meshWrapper,
        const shared_ptr<BoundingVolume> boundingVolume,
        const string& name = string(""),
        const Transform& transform = Transform()
    );

    Rigidbody(const Rigidbody& rigidbody);

    Rigidbody(Rigidbody&& rigidbody);

    Rigidbody& operator=(const Rigidbody& rigidbody) noexcept;

    Rigidbody& operator=(Rigidbody&& rigidbody) noexcept;

    void setMeshWrapper(const shared_ptr<MeshWrapper>& meshWrapper);

    shared_ptr<MeshWrapper>& getMeshWrapper(void) noexcept;

    void setBoundingVolume(const shared_ptr<BoundingVolume>& boundingVolume);

    shared_ptr<BoundingVolume>& getBoundingVolume(void) noexcept;
};

#endif // !RIGIDBODY_HPP
