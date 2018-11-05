#ifndef LIGHT_FACTORY_HPP
#define LIGHT_FACTORY_HPP

#include <PointLight.hpp>
#include <DirectionalLight.hpp>
#include <SpotLight.hpp>

#include <algorithm>

class LightFactory {
private:
    static vector<shared_ptr<PointLight>> pointLights;
    static vector<shared_ptr<DirectionalLight>> directionalLights;
    static vector<shared_ptr<SpotLight>> spotLights;

public:
    shared_ptr<PointLight> addPointLight(
        const vec3& ambient,
        const vec3& diffuse,
        const vec3& specular,
        const vec3& position,
        const GLfloat& constant,
        const GLfloat& linear,
        const GLfloat& quadratic
    ) noexcept;

    shared_ptr<DirectionalLight> addDirectionalLight(
        const vec3& ambient,
        const vec3& diffuse,
        const vec3& specular,
        const vec3& direction
    ) noexcept;

    shared_ptr<SpotLight> addSpotLight(
        const vec3& ambient,
        const vec3& diffuse,
        const vec3& specular,
        const vec3& position,
        const vec3& direction,
        const GLfloat& cutOff
    ) noexcept;

    static void loadLightsIntoShader(const shared_ptr<Shader>& shader) noexcept;
};

#endif // !LIGHT_FACTORY_HPP
