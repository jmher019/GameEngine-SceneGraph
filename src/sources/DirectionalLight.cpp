#include <DirectionalLight.hpp>

DirectionalLight::DirectionalLight(const string& name, const Transform& transform):
    Light(name, transform) {
}

DirectionalLight::DirectionalLight(const DirectionalLight& directionalLight):
    Light(directionalLight.name, directionalLight.transform) {
    ambient = directionalLight.ambient;
    diffuse = directionalLight.diffuse;
    specular = directionalLight.specular;
    direction = directionalLight.direction;
    children = directionalLight.children;
}

DirectionalLight::DirectionalLight(DirectionalLight&& directionalLight) {
    name = move(directionalLight.name);
    transform = move(directionalLight.transform);
    ambient = move(directionalLight.ambient);
    diffuse = move(directionalLight.diffuse);
    specular = move(directionalLight.specular);
    direction = move(directionalLight.direction);
    children = move(directionalLight.children);
}

DirectionalLight& DirectionalLight::operator=(const DirectionalLight& directionalLight) noexcept {
    name = directionalLight.name;
    transform = directionalLight.transform;
    ambient = directionalLight.ambient;
    diffuse = directionalLight.diffuse;
    specular = directionalLight.specular;
    direction = directionalLight.direction;
    children = directionalLight.children;

    return *this;
}

DirectionalLight& DirectionalLight::operator=(DirectionalLight&& directionalLight) noexcept {
    name = move(directionalLight.name);
    transform = move(directionalLight.transform);
    ambient = move(directionalLight.ambient);
    diffuse = move(directionalLight.diffuse);
    specular = move(directionalLight.specular);
    direction = move(directionalLight.direction);
    children = move(directionalLight.children);

    return *this;
}

const vec3& DirectionalLight::getDirection(void) const noexcept {
    return direction;
}

void DirectionalLight::setDirection(const vec3& direction) noexcept {
    this->direction = direction;
}

void DirectionalLight::loadIntoShader(const shared_ptr<Shader>& shader, const size_t& index) const noexcept {
    const string uniformName = string("directionalLights[") + to_string(index) + string("].");

    shader->setVec3(uniformName + string("ambient"), value_ptr(ambient));
    shader->setVec3(uniformName + string("diffuse"), value_ptr(diffuse));
    shader->setVec3(uniformName + string("specular"), value_ptr(specular));
    shader->setVec3(uniformName + string("direction"), value_ptr(direction));
}