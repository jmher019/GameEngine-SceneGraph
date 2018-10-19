#include <CubeMesh.hpp>

CubeMesh::CubeMesh(const string& name, const Transform& transform):
    Mesh(vector<Vertex>(36, Vertex()), name, transform) {
    
    // +x transformed face
    vertices[0] = Vertex(vec3(0.5f, -0.5f, 0.5f), vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 1.f));
    vertices[1] = Vertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 1.f));
    vertices[2] = Vertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 0.f));
    vertices[3] = Vertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 1.f));
    vertices[4] = Vertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 0.f));
    vertices[5] = Vertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 0.f));

    // -x transformed face
    vertices[6] = Vertex(vec3(-0.5f, -0.5f, 0.5f), vec3(-1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 1.f));
    vertices[7] = Vertex(vec3(-0.5f, 0.5f, 0.5f), vec3(-1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 0.f));
    vertices[8] = Vertex(vec3(-0.5f, -0.5f, -0.5f), vec3(-1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 1.f));
    vertices[9] = Vertex(vec3(-0.5f, -0.5f, -0.5f), vec3(-1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 1.f));
    vertices[10] = Vertex(vec3(-0.5f, 0.5f, 0.5f), vec3(-1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 0.f));
    vertices[11] = Vertex(vec3(-0.5f, 0.5f, -0.5f), vec3(-1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 0.f));

    // +y transformed face
    vertices[12] = Vertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 1.f));
    vertices[13] = Vertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 1.f));
    vertices[14] = Vertex(vec3(-0.5f, 0.5f, -0.5f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 0.f));
    vertices[15] = Vertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 1.f));
    vertices[16] = Vertex(vec3(0.5f, 0.5f, -0.5f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 0.f));
    vertices[17] = Vertex(vec3(-0.5f, 0.5f, -0.5f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 0.f));

    // -y transformed face
    vertices[18] = Vertex(vec3(-0.5f, -0.5f, 0.5f), vec3(0.f, -1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 0.f));
    vertices[19] = Vertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.f, -1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 1.f));
    vertices[20] = Vertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.f, -1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 0.f));
    vertices[21] = Vertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.f, -1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 0.f));
    vertices[22] = Vertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.f, -1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 1.f));
    vertices[23] = Vertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.f, -1.f, 0.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 1.f));

    // +z transformed face
    vertices[24] = Vertex(vec3(-0.5f, -0.5f, 0.5f), vec3(0.f, 0.f, 1.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 1.f));
    vertices[25] = Vertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.f, 0.f, 1.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 1.f));
    vertices[26] = Vertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.f, 0.f, 1.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 0.f));
    vertices[27] = Vertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.f, 0.f, 1.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 1.f));
    vertices[28] = Vertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.f, 0.f, 1.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 0.f));
    vertices[29] = Vertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.f, 0.f, 1.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 0.f));

    // -z transformed face
    vertices[30] = Vertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.f, 0.f, -1.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 1.f));
    vertices[31] = Vertex(vec3(-0.5f, 0.5f, -0.5f), vec3(0.f, 0.f, -1.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 0.f));
    vertices[32] = Vertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.f, 0.f, -1.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 1.f));
    vertices[33] = Vertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.f, 0.f, -1.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 1.f));
    vertices[34] = Vertex(vec3(-0.5f, 0.5f, -0.5f), vec3(0.f, 0.f, -1.f), vec3(0.f, 0.f, 0.f), vec2(1.f, 0.f));
    vertices[35] = Vertex(vec3(0.5f, 0.5f, -0.5f), vec3(0.f, 0.f, -1.f), vec3(0.f, 0.f, 0.f), vec2(0.f, 0.f));

    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

CubeMesh::CubeMesh(const CubeMesh& cubeMesh):
    Mesh(vector<Vertex>(), cubeMesh.name, cubeMesh.transform) {
    deallocate();

    vertices = cubeMesh.vertices;
    shader = cubeMesh.shader;
}

CubeMesh::CubeMesh(CubeMesh&& cubeMesh):
    Mesh(vector<Vertex>()) {
    deallocate();

    name = move(cubeMesh.name);
    transform = move(cubeMesh.transform);
    vertices = move(cubeMesh.vertices);
    VBO = move(cubeMesh.VBO);
    VAO = move(cubeMesh.VAO);
    shader = move(cubeMesh.shader);
}

CubeMesh& CubeMesh::operator=(const CubeMesh& cubeMesh) noexcept {
    name = cubeMesh.name;
    transform = cubeMesh.transform;
    shader = cubeMesh.shader;

    return *this;
}

CubeMesh& CubeMesh::operator=(CubeMesh&& cubeMesh) noexcept {
    deallocate();

    name = move(cubeMesh.name);
    transform = move(cubeMesh.transform);
    vertices = move(cubeMesh.vertices);
    VBO = move(cubeMesh.VBO);
    VAO = move(cubeMesh.VAO);
    shader = move(cubeMesh.shader);

    return *this;
}