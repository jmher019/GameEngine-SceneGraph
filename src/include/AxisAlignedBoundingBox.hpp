#ifndef AXIS_ALIGNED_BOUNDING_BOX
#define AXIS_ALIGNED_BOUNDING_BOX

#include <Triangle.hpp>
#include <BoundingSphere.hpp>

class AxisAlignedBoundingBox : public BoundingVolume {
private:
    vec3 initialHalfExtents = vec3(0.f, 0.f, 0.f);
    vec3 halfExtents = vec3(0.f, 0.f, 0.f);

protected:
    vector<Vertex> getVerticesForGrid(void) const noexcept override;

public:
    AxisAlignedBoundingBox(const vec3& initialHalfExtents, const string& name = string(""), const Transform& transform = Transform());

    AxisAlignedBoundingBox(const AxisAlignedBoundingBox& aabb);

    AxisAlignedBoundingBox(AxisAlignedBoundingBox&& aabb);

    AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& sphere) noexcept;

    AxisAlignedBoundingBox& operator=(AxisAlignedBoundingBox&& sphere) noexcept;

    const vec3& getHalfExtents(void) const noexcept;

    void setHalfExtents(const vec3& halfExtents) noexcept;

    GLfloat getSquaredDistancePtPointAABB(const vec3& pt) const noexcept;

    vector<Triangle> getTriangles(void) const;

    bool intersectsVolume(BoundingVolume*& boundingVolume) const noexcept override;

    bool enclosesVolume(BoundingVolume*& boundingVolume) const noexcept override;

    bool isEnclosedByVolume(BoundingVolume*& boundingVolume) const noexcept override;

    void update(const Transform& newTransform) override;
};

#endif // !AXIS_ALIGNED_BOUNDING_BOX
