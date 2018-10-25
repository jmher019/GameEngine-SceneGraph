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

void Rigidbody::update(const float& deltaSeconds) {
    setVelocity(velocity + acceleration * deltaSeconds);
    const vec3 translation = velocity * deltaSeconds;
    translate(translation.x, translation.y, translation.z);
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

void Rigidbody::setIsStatic(const GLboolean& isStatic) noexcept {
    this->isStatic = isStatic;
}

const GLboolean& Rigidbody::getIsStatic(void) const noexcept {
    return isStatic;
}

void Rigidbody::setVelocity(const vec3& velocity) noexcept {
    this->velocity = velocity;
}

const vec3& Rigidbody::getVelocity(void) const noexcept {
    return velocity;
}

const vec3& Rigidbody::getAcceleration(void) const noexcept {
    return acceleration;
}

void Rigidbody::setAcceleration(const vec3& acceleration) noexcept {
    this->acceleration;
}

void Rigidbody::applyForce(const vec3& force) noexcept {
    setAcceleration(force / mass);
}

const GLfloat& Rigidbody::getMass(void) const noexcept {
    return mass;
}

void Rigidbody::setMass(const GLfloat& mass) noexcept {
    this->mass = mass;
}

void Rigidbody::handleCollision(Rigidbody* rigidbody) noexcept {
    Contact contact = move(CollisionDetector::isVolumeIntersectingVolume(boundingVolume.get(), rigidbody->getBoundingVolume().get()));
    if (contact.getContactValidity() == ContactValidity::VALID) {
        if (!isStatic && contact.getPenetration() > GeometryUtils::epsilon) {
            const vec3& normal = contact.getContactNormal();
            const GLfloat& penetration = contact.getPenetration();
            const vec3 translationVector = normal * penetration;

            translate(translationVector.x, translationVector.y, translationVector.z);
            setVelocity(velocity - dot(normal, velocity) * normal);
        }
    }
}