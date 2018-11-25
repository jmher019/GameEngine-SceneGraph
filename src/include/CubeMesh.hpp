#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP

#include <Mesh.hpp>

class CubeMesh : public Mesh {
public:
    CubeMesh(const string& name = SceneUtils::createId(), const Transform& transform = Transform());

    CubeMesh(const CubeMesh& cubeMesh);

    CubeMesh(CubeMesh&& cubeMesh);

    CubeMesh& operator=(const CubeMesh& cubeMesh) noexcept;

    CubeMesh& operator=(CubeMesh&& cubeMesh) noexcept;
};

#endif // !CUBE_MESH_HPP
