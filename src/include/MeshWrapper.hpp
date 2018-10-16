#ifndef MESH_WRAPPER_HPP
#define MESH_WRAPPER_HPP

#include <Mesh.hpp>

class MeshWrapper : public SceneObject {
private:
    shared_ptr<Mesh> mesh;

public:
    MeshWrapper(const shared_ptr<Mesh>& mesh, const string& name = string(""), const Transform& transform = Transform());

    MeshWrapper(const MeshWrapper& meshWrapper);

    MeshWrapper(MeshWrapper&& meshWrapper);

    MeshWrapper& operator=(const MeshWrapper& meshWrapper) noexcept;

    MeshWrapper& operator=(MeshWrapper&& meshWrapper) noexcept;

    void setMeshWrapper(const shared_ptr<Mesh>& mesh) noexcept;

    const shared_ptr<Mesh>& getMeshWrapper(void) const noexcept;

    void draw(const mat4& ProjectionViewMatrix) const override;
};

#endif // !MESH_WRAPPER_HPP
