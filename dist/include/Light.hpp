#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <Shader.hpp>
#include <SceneObject.hpp>
#include <SceneUtils.hpp>

class Light : public SceneObject {
protected:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    Light(const string& name = SceneUtils::createId(), const Transform& transform = Transform()):
        SceneObject(name, transform) {
    };

public:
    virtual void loadIntoShader(const shared_ptr<Shader>& shader, const size_t& index) const noexcept = 0;

    const vec3& getAmbient(void) const noexcept {
        return ambient;
    };

    void setAmbient(const vec3& ambient) noexcept {
        this->ambient = ambient;
    };

    const vec3& getDiffuse(void) const noexcept {
        return diffuse;
    };

    void setDiffuse(const vec3& diffuse) noexcept {
        this->diffuse = diffuse;
    };

    const vec3& getSpecular(void) const noexcept {
        return specular;
    };

    void setSpecular(const vec3& specular) noexcept {
        this->specular = specular;
    };
};

#endif // !LIGHT_HPP
