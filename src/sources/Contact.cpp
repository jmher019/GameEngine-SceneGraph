#include <Contact.hpp>

Contact::Contact(
    const vec3& contactPoint,
    const vec3& contactNormal,
    const GLfloat& penetration,
    const ContactValidity& validity
):
    contactPoint(contactPoint),
    contactNormal(contactNormal),
    penetration(penetration),
    validity(validity) {
}

Contact::Contact(const Contact& contact):
    contactPoint(contact.contactPoint),
    contactNormal(contact.contactNormal),
    penetration(contact.penetration),
    validity(contact.validity) {
}

Contact::Contact(Contact&& contact) {
    contactPoint = move(contact.contactPoint);
    contactNormal = move(contact.contactNormal);
    penetration = move(contact.penetration);
    validity = move(contact.validity);
}

Contact& Contact::operator=(const Contact& contact) noexcept {
    contactPoint = contact.contactPoint;
    contactNormal = contact.contactNormal;
    penetration = contact.penetration;
    validity = contact.validity;

    return *this;
}

Contact& Contact::operator=(Contact&& contact) noexcept {
    contactPoint = move(contact.contactPoint);
    contactNormal = move(contact.contactNormal);
    penetration = move(contact.penetration);
    validity = move(contact.validity);

    return *this;
}

const vec3& Contact::getContactPoint(void) const noexcept {
    return contactPoint;
}

void Contact::setContactPoint(const vec3& contactPoint) noexcept {
    this->contactPoint = contactPoint;
}

const vec3& Contact::getContactNormal(void) const noexcept {
    return contactNormal;
}

void Contact::setContactNormal(const vec3& contactNormal) noexcept {
    this->contactNormal = contactNormal;
}

const GLfloat& Contact::getPenetration(void) const noexcept {
    return penetration;
}

void Contact::setPenetration(const GLfloat& penetration) noexcept {
    this->penetration = penetration;
}

const ContactValidity& Contact::getContactValidity(void) const noexcept {
    return validity;
}

void Contact::setContactValidity(const ContactValidity& validity) noexcept {
    this->validity = validity;
}