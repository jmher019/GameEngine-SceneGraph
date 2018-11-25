#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include <Light.hpp>
#include <glm/gtc/type_ptr.hpp>

class SpotLight : public Light {
private:
    vec3 position;
    vec3 direction;
    GLfloat cutOff;

public:
    SpotLight(const string& name = SceneUtils::createId(), const Transform& transform = Transform());

    SpotLight(const SpotLight& spotLight);

    SpotLight(SpotLight&& spotLight);

    SpotLight& operator=(const SpotLight& spotLight) noexcept;

    SpotLight& operator=(SpotLight&& spotLight) noexcept;

    const vec3& getPosition(void) const noexcept;

    void setPosition(const vec3& position) noexcept;

    const vec3& getDirection(void) const noexcept;

    void setDirection(const vec3& direction) noexcept;

    const GLfloat& getCutOff(void) const noexcept;

    void setCutOff(const GLfloat& cutOff) noexcept;

    void loadIntoShader(const shared_ptr<Shader>& shader, const size_t& index) const noexcept override;
};

#endif // !SPOT_LIGHT_HPP
