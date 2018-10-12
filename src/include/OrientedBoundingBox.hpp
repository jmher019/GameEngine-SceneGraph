#ifndef ORIENTED_BOUNDING_BOX
#define ORIENTED_BOUNDING_BOX

#include <BoundingCapsule.hpp>

class OrientedBoundingBox : public BoundingVolume {
private:
    vec3 halfExtents = vec3(0.f, 0.f, 0.f);
    vec3 xAxis = vec3(1.f, 0.f, 0.f);
    vec3 yAxis = vec3(0.f, 1.f, 0.f);
    vec3 zAxis = vec3(0.f, 0.f, 1.f);

protected:
    vector<Vertex> getVerticesForGrid(void) const noexcept override;

public:
    OrientedBoundingBox(const vec3& halfExtents, const string& name = string(""), const Transform& transform = Transform());

    OrientedBoundingBox(const OrientedBoundingBox& obb);

    OrientedBoundingBox(OrientedBoundingBox&& obb);

    OrientedBoundingBox& operator=(const OrientedBoundingBox& obb) noexcept;

    OrientedBoundingBox& operator=(OrientedBoundingBox&& obb) noexcept;

    const vec3& getHalfExtents(void) const noexcept;

    const vec3& getXAxis(void) const noexcept;

    const vec3& getYAxis(void) const noexcept;

    const vec3& getZAxis(void) const noexcept;

    vector<Triangle> getTriangles(void) const noexcept;

    vec3 getClosestPtPointOBB(const vec3& pt) const noexcept;

    bool intersectsVolume(BoundingVolume*& boundingVolume) const noexcept override;

    bool enclosesVolume(BoundingVolume*& boundingVolume) const noexcept override;

    bool isEnclosedByVolume(BoundingVolume*& boundingVolume) const noexcept override;

    void update(const Transform& newTransform) override;
};

#endif // !ORIENTED_BOUNDING_BOX
