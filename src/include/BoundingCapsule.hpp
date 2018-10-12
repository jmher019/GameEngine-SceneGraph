#ifndef BOUNDING_CAPSULE
#define BOUNDING_CAPSULE

#include <Line.hpp>
#include <AxisAlignedBoundingBox.hpp>

class BoundingCapsule : public BoundingVolume {
private:
    static constexpr GLint POINTS_PER_RING = 60;

    GLfloat capsuleLineLength;
    GLfloat radius;

protected:
    vector<Vertex> getVerticesForGrid(void) const noexcept override;

public:
    BoundingCapsule(const GLfloat& capsuleLineLength, const GLfloat& radius, const string& name = string(""), const Transform& transform = Transform());

    BoundingCapsule(const BoundingCapsule& capsule);

    BoundingCapsule(BoundingCapsule&& capsule);

    BoundingCapsule& operator=(const BoundingCapsule& capsule) noexcept;

    BoundingCapsule& operator=(BoundingCapsule&& capsule) noexcept;

    const GLfloat& getCapsuleLineLength(void) const noexcept;

    const GLfloat& getRadius(void) const noexcept;

    bool intersectsVolume(BoundingVolume*& boundingVolume) const noexcept override;

    bool enclosesVolume(BoundingVolume*& boundingVolume) const noexcept override;

    bool isEnclosedByVolume(BoundingVolume*& boundingVolume) const noexcept override;

    void update(const Transform& newTransform) override;
};

#endif // !BOUNDING_CAPSULE
