#ifndef BOUNDING_SPHERE_HPP
#define BOUNDING_SPHERE_HPP

#include <BoundingVolume.hpp>
#include <GeometryUtils.hpp>

class BoundingSphere : public BoundingVolume {
private:
    static constexpr GLint POINTS_PER_RING = 16;

    GLfloat radius = 0.f;

protected:
    vector<Vertex> getVerticesForGrid(void) const noexcept override;

public:
    BoundingSphere(const GLfloat& radius, const string& name = string(""), const Transform& transform = Transform());

    BoundingSphere(const BoundingSphere& sphere);

    BoundingSphere(BoundingSphere&& sphere);

    BoundingSphere& operator=(const BoundingSphere& sphere) noexcept;

    BoundingSphere& operator=(BoundingSphere&& sphere) noexcept;

    const GLfloat& getRadius(void) const noexcept;

    void setRadius(const GLfloat& radius) noexcept;

    GLfloat getActualRadius(void) const noexcept;

    void draw(const mat4& ProjectionViewMatrix) const override;
};

#endif // !BOUNDING_SPHERE_HPP
