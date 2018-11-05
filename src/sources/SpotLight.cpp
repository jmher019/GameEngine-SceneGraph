#include <SpotLight.hpp>

SpotLight::SpotLight(const string& name, const Transform& transform) :
    Light(name, transform) {
}

SpotLight::SpotLight(const SpotLight& spotLight) :
    Light(spotLight.name, spotLight.transform) {
    ambient = spotLight.ambient;
    diffuse = spotLight.diffuse;
    specular = spotLight.specular;
    position = spotLight.position;
    direction = spotLight.direction;
    cutOff = spotLight.cutOff;
    children = spotLight.children;
}

SpotLight::SpotLight(SpotLight&& spotLight) {
    name = move(spotLight.name);
    transform = move(spotLight.transform);
    ambient = move(spotLight.ambient);
    diffuse = move(spotLight.diffuse);
    specular = move(spotLight.specular);
    position = move(spotLight.position);
    direction = move(spotLight.direction);
    cutOff = move(spotLight.cutOff);
    children = move(spotLight.children);
}

SpotLight& SpotLight::operator=(const SpotLight& spotLight) noexcept {
    name = spotLight.name;
    transform = spotLight.transform;
    ambient = spotLight.ambient;
    diffuse = spotLight.diffuse;
    specular = spotLight.specular;
    position = spotLight.position;
    direction = spotLight.direction;
    cutOff = spotLight.cutOff;
    children = spotLight.children;

    return *this;
}

SpotLight& SpotLight::operator=(SpotLight&& spotLight) noexcept {
    name = move(spotLight.name);
    transform = move(spotLight.transform);
    ambient = move(spotLight.ambient);
    diffuse = move(spotLight.diffuse);
    specular = move(spotLight.specular);
    position = move(spotLight.position);
    direction = move(spotLight.direction);
    cutOff = move(spotLight.cutOff);
    children = move(spotLight.children);

    return *this;
}

const vec3& SpotLight::getPosition(void) const noexcept {
    return position;
}

void SpotLight::setPosition(const vec3& position) noexcept {
    this->position = position;
}

const vec3& SpotLight::getDirection(void) const noexcept {
    return direction;
}

void SpotLight::setDirection(const vec3& direction) noexcept {
    this->direction = direction;
}

const GLfloat& SpotLight::getCutOff(void) const noexcept {
    return cutOff;
}

void SpotLight::setCutOff(const GLfloat& cutOff) noexcept {
    this->cutOff = cutOff;
}

void SpotLight::loadIntoShader(const shared_ptr<Shader>& shader, const size_t& index) const noexcept {
    const string uniformName = string("spotLights[") + to_string(index) + string("].");

    shader->setVec3(uniformName + string("ambient"), value_ptr(ambient));
    shader->setVec3(uniformName + string("diffuse"), value_ptr(diffuse));
    shader->setVec3(uniformName + string("specular"), value_ptr(specular));
    shader->setVec3(uniformName + string("position"), value_ptr(position));
    shader->setVec3(uniformName + string("direction"), value_ptr(direction));
    shader->setFloat(uniformName + string("cutOff"), cutOff);
}