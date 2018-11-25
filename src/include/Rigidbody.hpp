#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <MeshWrapper.hpp>
#include <CollisionDetector.hpp>

#include <stdexcept>

class Rigidbody : public SceneObject {
private:
    shared_ptr<MeshWrapper> meshWrapper;
    shared_ptr<BoundingVolume> boundingVolume;
    vec3 velocity = vec3(0.f, 0.f, 0.f);
    vec3 acceleration = vec3(0.f, 0.f, 0.f);
    GLfloat mass = 1.f;
    GLboolean isStatic = false;

public:
    Rigidbody(
        const shared_ptr<MeshWrapper> meshWrapper,
        const shared_ptr<BoundingVolume> boundingVolume,
        const string& name = SceneUtils::createId(),
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

    void setIsStatic(const GLboolean& isStatic) noexcept;

    const GLboolean& getIsStatic(void) const noexcept;

    const vec3& getVelocity(void) const noexcept;

    void setVelocity(const vec3& velocity) noexcept;

    const vec3& getAcceleration(void) const noexcept;

    void setAcceleration(const vec3& acceleration) noexcept;

    void applyForce(const vec3& force) noexcept;

    const GLfloat& getMass(void) const noexcept;

    void setMass(const GLfloat& mass) noexcept;
        
    virtual void handleCollision(Rigidbody* rigidbody, const GLfloat& deltaSeconds) noexcept;
};

#endif // !RIGIDBODY_HPP
