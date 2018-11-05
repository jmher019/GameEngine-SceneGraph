#include <LightFactory.hpp>

shared_ptr<PointLight> LightFactory::addPointLight(
    const vec3& ambient,
    const vec3& diffuse,
    const vec3& specular,
    const vec3& position,
    const GLfloat& constant,
    const GLfloat& linear,
    const GLfloat& quadratic
) noexcept {
    shared_ptr<PointLight> pointLight = make_shared<PointLight>();
    pointLight->setAmbient(ambient);
    pointLight->setDiffuse(diffuse);
    pointLight->setSpecular(specular);
    pointLight->setPosition(position);
    pointLight->setConstant(constant);
    pointLight->setLinear(linear);
    pointLight->setQuadratic(quadratic);

    pointLights.push_back(pointLight);

    return pointLight;
}

shared_ptr<DirectionalLight> LightFactory::addDirectionalLight(
    const vec3& ambient,
    const vec3& diffuse,
    const vec3& specular,
    const vec3& direction
) noexcept {
    shared_ptr<DirectionalLight> directionalLight = make_shared<DirectionalLight>();
    directionalLight->setAmbient(ambient);
    directionalLight->setDiffuse(diffuse);
    directionalLight->setSpecular(specular);
    directionalLight->setDirection(direction);

    directionalLights.push_back(directionalLight);

    return directionalLight;
}

shared_ptr<SpotLight> LightFactory::addSpotLight(
    const vec3& ambient,
    const vec3& diffuse,
    const vec3& specular,
    const vec3& position,
    const vec3& direction,
    const GLfloat& cutOff
) noexcept {
    shared_ptr<SpotLight> spotLight = make_shared<SpotLight>();
    spotLight->setAmbient(ambient);
    spotLight->setDiffuse(diffuse);
    spotLight->setSpecular(specular);
    spotLight->setPosition(position);
    spotLight->setDirection(direction);
    spotLight->setCutOff(cutOff);

    spotLights.push_back(spotLight);

    return spotLight;
}

void LightFactory::loadLightsIntoShader(const shared_ptr<Shader>& shader) noexcept {
    shader->setUInt("pointLightsLength", static_cast<GLuint>(pointLights.size()));
    size_t i = 0;
    for_each(pointLights.begin(), pointLights.end(), [&](const shared_ptr<PointLight>& pointLight) {
        pointLight->loadIntoShader(shader, i);
        ++i;
    });

    shader->setUInt("directionalLightsLength", static_cast<GLuint>(directionalLights.size()));
    i = 0;
    for_each(directionalLights.begin(), directionalLights.end(), [&](const shared_ptr<DirectionalLight>& directionalLight) {
        directionalLight->loadIntoShader(shader, i);
        ++i;
    });

    shader->setUInt("spotLightsLength", static_cast<GLuint>(spotLights.size()));
    i = 0;
    for_each(spotLights.begin(), spotLights.end(), [&](const shared_ptr<SpotLight>& spotLight) {
        spotLight->loadIntoShader(shader, i);
        ++i;
    });
}