#ifndef BOUNDING_CAPSULE
#define BOUNDING_CAPSULE

#include <Line.hpp>
#include <BoundingSphere.hpp>

class BoundingCapsule : public BoundingVolume {
private:
    static constexpr GLint POINTS_PER_RING = 16;

    GLuint lineVAO = 0;
    GLuint lineVBO = 0;
    GLfloat capsuleLineLength;
    GLfloat radius;
    vector<Vertex> lineVertices;
    shared_ptr<Shader> capsuleLineShader = nullptr;

    vector<Vertex> getVerticesForLine(void) const noexcept;

protected:
    vector<Vertex> getVerticesForGrid(void) const noexcept override;

    void initialize(void) noexcept override;

    void deallocate(void) noexcept override;

public:
    BoundingCapsule(const GLfloat& capsuleLineLength, const GLfloat& radius, const string& name = string(""), const Transform& transform = Transform());

    BoundingCapsule(const BoundingCapsule& capsule);

    BoundingCapsule(BoundingCapsule&& capsule);

    BoundingCapsule& operator=(const BoundingCapsule& capsule) noexcept;

    BoundingCapsule& operator=(BoundingCapsule&& capsule) noexcept;

    const GLfloat& getCapsuleLineLength(void) const noexcept;

    void setCapsuleLineLength(const GLfloat& capsuleLineLength) noexcept;

    const GLfloat& getRadius(void) const noexcept;

    void setRadius(const GLfloat& radius) noexcept;

    Line getActualLine(void) const noexcept;

    GLfloat getActualRadius(void) const noexcept;

    GLfloat getActualCapsuleLineLength(void) const noexcept;

    bool intersectsVolume(BoundingVolume* boundingVolume) const noexcept override;

    bool enclosesVolume(BoundingVolume* boundingVolume) const noexcept override;

    bool isEnclosedByVolume(BoundingVolume* boundingVolume) const noexcept override;

    void setCapsuleLineShader(const shared_ptr<Shader>& capsuleLineShader) noexcept;

    void draw(const mat4& ProjectionViewMatrix) const override;
};

#endif // !BOUNDING_CAPSULE
