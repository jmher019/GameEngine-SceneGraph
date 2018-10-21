#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <Vertex.hpp>

enum class ContactValidity {
    INVALID,
    VALID
};

class Contact {
private:
    vec3 contactPoint;
    vec3 contactNormal;
    GLfloat penetration;
    ContactValidity validity;

public:
    Contact(
        const vec3& contactPoint,
        const vec3& contactNormal,
        const GLfloat& penetration,
        const ContactValidity& validity
    );

    Contact(const Contact& contact);

    Contact(Contact&& contact);

    Contact& operator=(const Contact& contact) noexcept;

    Contact& operator=(Contact&& contact) noexcept;

    const vec3& getContactPoint(void) const noexcept;

    void setContactPoint(const vec3& contactPoint) noexcept;

    const vec3& getContactNormal(void) const noexcept;

    void setContactNormal(const vec3& contactNormal) noexcept;

    const GLfloat& getPenetration(void) const noexcept;

    void setPenetration(const GLfloat& penetration) noexcept;

    const ContactValidity& getContactValidity(void) const noexcept;

    void setContactValidity(const ContactValidity& validity) noexcept;
};

#endif // !CONTACT_HPP
