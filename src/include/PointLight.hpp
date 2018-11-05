#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <Light.hpp>
#include <glm/gtc/type_ptr.hpp>

class PointLight : public Light {
private:
    vec3 position;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;

public:
    PointLight(const string& name = string(""), const Transform& transform = Transform());

    PointLight(const PointLight& pointLight);

    PointLight(PointLight&& pointLight);

    PointLight& operator=(const PointLight& pointLight) noexcept;

    PointLight& operator=(PointLight&& pointLight) noexcept;

    const vec3& getPosition(void) const noexcept;

    void setPosition(const vec3& position) noexcept;

    const GLfloat& getConstant(void) const noexcept;

    void setConstant(const GLfloat& constant) noexcept;

    const GLfloat& getLinear(void) const noexcept;

    void setLinear(const GLfloat& linear) noexcept;

    const GLfloat& getQuadratic(void) const noexcept;

    void setQuadratic(const GLfloat& quadratic) noexcept;

    void loadIntoShader(const shared_ptr<Shader>& shader, const size_t& index) const noexcept override;
};

#endif // !POINT_LIGHT_HPP
