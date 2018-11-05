#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include <Light.hpp>
#include <glm/gtc/type_ptr.hpp>

class DirectionalLight : public Light {
private:
    vec3 direction;

public:
    DirectionalLight(const string& name = string(""), const Transform& transform = Transform());

    DirectionalLight(const DirectionalLight& directionalLight);

    DirectionalLight(DirectionalLight&& directionalLight);

    DirectionalLight& operator=(const DirectionalLight& directionalLight) noexcept;

    DirectionalLight& operator=(DirectionalLight&& directionalLight) noexcept;

    const vec3& getDirection(void) const noexcept;

    void setDirection(const vec3& direction) noexcept;

    void loadIntoShader(const shared_ptr<Shader>& shader, const size_t& index) const noexcept override;
};

#endif // !DIRECTIONAL_LIGHT_HPP
