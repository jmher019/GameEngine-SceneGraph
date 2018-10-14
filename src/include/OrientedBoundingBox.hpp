#ifndef ORIENTED_BOUNDING_BOX
#define ORIENTED_BOUNDING_BOX

#include <BoundingCapsule.hpp>
#include <Triangle.hpp>

class OrientedBoundingBox : public BoundingVolume {
private:
    vec3 halfExtents = vec3(0.f, 0.f, 0.f);

protected:
    vector<Vertex> getVerticesForGrid(void) const noexcept override;

public:
    OrientedBoundingBox(const vec3& halfExtents, const string& name = string(""), const Transform& transform = Transform());

    OrientedBoundingBox(const OrientedBoundingBox& obb);

    OrientedBoundingBox(OrientedBoundingBox&& obb);

    OrientedBoundingBox& operator=(const OrientedBoundingBox& obb) noexcept;

    OrientedBoundingBox& operator=(OrientedBoundingBox&& obb) noexcept;

    const vec3& getHalfExtents(void) const noexcept;

    void setHalfExtents(const vec3& halfExtents) noexcept;

    vec3 getActualHalfExtents(void) const noexcept;

    vec3 getXAxis(void) const noexcept;

    vec3 getYAxis(void) const noexcept;

    vec3 getZAxis(void) const noexcept;

    vector<Triangle> getTriangles(void) const noexcept;

    vec3 getClosestPtPointOBB(const vec3& pt) const noexcept;

    bool intersectsVolume(BoundingVolume* boundingVolume) const noexcept override;

    bool enclosesVolume(BoundingVolume* boundingVolume) const noexcept override;

    bool isEnclosedByVolume(BoundingVolume* boundingVolume) const noexcept override;

    void draw(const mat4& ProjectionViewMatrix) const override;
};

#endif // !ORIENTED_BOUNDING_BOX
