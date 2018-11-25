#include <iostream>
#include <iomanip>

#include <Mesh.hpp>
#include <SceneGraph.hpp>

using namespace std;

int mainFake() {
    cout << "sizeof Transform: " << sizeof(Transform) << endl;
    cout << "sizeof SceneObject: " << sizeof(SceneObject) << endl;
    cout << "sizeof Vertex: " << sizeof(Vertex) << endl;
    cout << "sizeof Mesh: " << sizeof(Mesh) << endl;
    cout << "sizeof SceneGraph: " << sizeof(SceneGraph) << endl;

    Transform transform(fdualquat(rotate(fquat(1.f, 0.f, 0.f, 0.f), glm::radians(50.f), vec3(0.f, 1.f, 0.f)), vec3(5.f, 6.f, 7.f)), vec3(2.f, 0.5, 3.f));

    mat4 M = translate(mat4(1.f), vec3(5.f, 6.f, 7.f)) * rotate(mat4(1.f), glm::radians(50.f), vec3(0.f, 1.f, 0.f)) * scale(mat4(1.f), vec3(2.f, 0.5f, 3.f));

    SceneObject sceneObject("Test", transform);

    SceneGraph graph(make_shared<SceneObject>(string("Another Test"), transform));

    system("pause");
    return EXIT_SUCCESS;
}