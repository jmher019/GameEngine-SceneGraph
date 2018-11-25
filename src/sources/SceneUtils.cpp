#include <SceneUtils.hpp>

unsigned long SceneUtils::counter = 0;

void SceneUtils::translate(SceneObject& sceneObject, const vec3& t) noexcept {
    sceneObject.setTransform(puggo::translate(sceneObject.getTransform(), t));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](const shared_ptr<SceneObject>& child) {
            SceneUtils::translate(*child.get(), t);  
        }
    );
}

void SceneUtils::translate(SceneObject& sceneObject, const float& tX, const float& tY, const float& tZ) noexcept {
    sceneObject.setTransform(puggo::translate(sceneObject.getTransform(), tX, tY, tZ));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](const shared_ptr<SceneObject>& child) {
            SceneUtils::translate(*child.get(), tX, tY, tZ);
        }
    );
}

void SceneUtils::rotate(SceneObject& sceneObject, const vec3& degrees) noexcept {
    sceneObject.setTransform(puggo::rotate(sceneObject.getTransform(), degrees));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](const shared_ptr<SceneObject>& child) {
            SceneUtils::rotate(*child.get(), degrees);
        }
    );
}

void SceneUtils::rotate(SceneObject& sceneObject, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    sceneObject.setTransform(puggo::rotate(sceneObject.getTransform(), degreesX, degreesY, degreesZ));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](const shared_ptr<SceneObject>& child) {
            SceneUtils::rotate(*child.get(), degreesX, degreesY, degreesZ);
        }
    );
}

void SceneUtils::orbit(SceneObject& sceneObject, const vec3& degrees) noexcept {
    sceneObject.setTransform(puggo::orbit(sceneObject.getTransform(), degrees));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](const shared_ptr<SceneObject>& child) {
            SceneUtils::orbit(*child.get(), degrees);
        }
    );
}

void SceneUtils::orbit(SceneObject& sceneObject, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    sceneObject.setTransform(puggo::orbit(sceneObject.getTransform(), degreesX, degreesY, degreesZ));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](const shared_ptr<SceneObject>& child) {
            SceneUtils::orbit(*child.get(), degreesX, degreesY, degreesZ);
        }
    );
}

void SceneUtils::scale(SceneObject& sceneObject, const vec3& s) noexcept {
    sceneObject.setTransform(puggo::scale(sceneObject.getTransform(), s));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](const shared_ptr<SceneObject>& child) {
            SceneUtils::scale(*child.get(), s);
        }
    );
}

void SceneUtils::scale(SceneObject& sceneObject, const float& sX, const float& sY, const float& sZ) noexcept {
    sceneObject.setTransform(puggo::scale(sceneObject.getTransform(), sX, sY, sZ));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](const shared_ptr<SceneObject>& child) {
            SceneUtils::scale(*child.get(), sX, sY, sZ);
        }
    );
}

void SceneUtils::appendChild(SceneObject& sceneObject, const shared_ptr<SceneObject>& newChild) noexcept {
    if (&sceneObject == newChild.get()) {
        return;
    }

    SceneUtils::removeChild(sceneObject, newChild);
    sceneObject.getChildren().push_back(newChild);
}

void SceneUtils::insertChild(SceneObject& sceneObject, const size_t& index, const shared_ptr<SceneObject>& newChild) noexcept {
    SceneUtils::removeChild(sceneObject, newChild);

    if (index >= sceneObject.getChildren().size()) {
        sceneObject.getChildren().insert(sceneObject.getChildren().begin() + index, newChild);
    }
    else if (!sceneObject.getChildren().empty()) {
        sceneObject.getChildren().push_back(newChild);
    }
}

void SceneUtils::removeChild(SceneObject& sceneObject, const shared_ptr<SceneObject>& newChild) noexcept {
    for (auto it = sceneObject.getChildren().begin(); it != sceneObject.getChildren().end(); ++it) {
        if ((*it) == newChild) {
            sceneObject.getChildren().erase(it);
            break;
        }
    }
}

void SceneUtils::removeChild(SceneObject& sceneObject, const size_t& index) noexcept {
    if (index >= sceneObject.getChildren().size()) {
        sceneObject.getChildren().erase(sceneObject.getChildren().begin() + index);
    }
    else if (!sceneObject.getChildren().empty()) {
        sceneObject.getChildren().pop_back();
    }
}

void SceneUtils::replaceChild(SceneObject& sceneObject, const size_t& index, const shared_ptr<SceneObject>& newChild) noexcept {
    SceneUtils::removeChild(sceneObject, index);
    SceneUtils::insertChild(sceneObject, index, newChild);
}

long SceneUtils::findChildIndex(SceneObject& sceneObject, const shared_ptr<SceneObject>& child) noexcept {
    long i = 0;
    for (auto it = sceneObject.getChildren().begin(); it != sceneObject.getChildren().end(); ++it, ++i) {
        if ((*it) == child) {
            return i;
        }
    }

    return -1;
}

string SceneUtils::createId(void) noexcept {
    return to_string(counter++);
}