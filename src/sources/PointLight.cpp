#include <PointLight.hpp>

PointLight::PointLight(const string& name, const Transform& transform) :
    Light(name, transform) {
}

PointLight::PointLight(const PointLight& pointLight):
    Light(pointLight.name, pointLight.transform) {
    ambient = pointLight.ambient;
    diffuse = pointLight.diffuse;
    specular = pointLight.specular;
    position = pointLight.position;
    constant = pointLight.constant;
    linear = pointLight.linear;
    quadratic = pointLight.quadratic;
    children = pointLight.children;
}

PointLight::PointLight(PointLight&& pointLight) {
    name = move(pointLight.name);
    transform = move(pointLight.transform);
    ambient = move(pointLight.ambient);
    diffuse = move(pointLight.diffuse);
    specular = move(pointLight.specular);
    position = move(pointLight.position);
    constant = move(pointLight.constant);
    linear = move(pointLight.linear);
    quadratic = move(pointLight.quadratic);
    children = move(pointLight.children);
}

PointLight& PointLight::operator=(const PointLight& pointLight) noexcept {
    name = pointLight.name;
    transform = pointLight.transform;
    ambient = pointLight.ambient;
    diffuse = pointLight.diffuse;
    specular = pointLight.specular;
    position = pointLight.position;
    constant = pointLight.constant;
    linear = pointLight.linear;
    quadratic = pointLight.quadratic;
    children = pointLight.children;

    return *this;
}

PointLight& PointLight::operator=(PointLight&& pointLight) noexcept {
    name = move(pointLight.name);
    transform = move(pointLight.transform);
    ambient = move(pointLight.ambient);
    diffuse = move(pointLight.diffuse);
    specular = move(pointLight.specular);
    position = move(pointLight.position);
    constant = move(pointLight.constant);
    linear = move(pointLight.linear);
    quadratic = move(pointLight.quadratic);
    children = move(pointLight.children);

    return *this;
}

const vec3& PointLight::getPosition(void) const noexcept {
    return position;
}

void PointLight::setPosition(const vec3& position) noexcept {
    this->position = position;
}

const GLfloat& PointLight::getConstant(void) const noexcept {
    return constant;
}

void PointLight::setConstant(const GLfloat& constant) noexcept {
    this->constant = constant;
}

const GLfloat& PointLight::getLinear(void) const noexcept {
    return linear;
}

void PointLight::setLinear(const GLfloat& linear) noexcept {
    this->linear = linear;
}

const GLfloat& PointLight::getQuadratic(void) const noexcept {
    return quadratic;
}

void PointLight::setQuadratic(const GLfloat& quadratic) noexcept {
    this->quadratic = quadratic;
}

void PointLight::loadIntoShader(const shared_ptr<Shader>& shader, const size_t& index) const noexcept {
    const string uniformName = string("pointLights[") + to_string(index) + string("].");

    shader->setVec3(uniformName + string("ambient"), value_ptr(ambient));
    shader->setVec3(uniformName + string("diffuse"), value_ptr(diffuse));
    shader->setVec3(uniformName + string("specular"), value_ptr(specular));
    shader->setVec3(uniformName + string("position"), value_ptr(position));
    shader->setFloat(uniformName + string("constant"), constant);
    shader->setFloat(uniformName + string("linear"), linear);
    shader->setFloat(uniformName + string("quadratic"), quadratic);
}