#include <SceneUtils.hpp>

unsigned long SceneUtils::counter = 0;

void SceneUtils::translate(SceneObject& sceneObject, const vec3& t) noexcept {
    sceneObject.setTransform(puggo::translate(sceneObject.getTransform(), t));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
        SceneUtils::translate(child, t);
    }
    );
}

void SceneUtils::translate(SceneObject& sceneObject, const float& tX, const float& tY, const float& tZ) noexcept {
    sceneObject.setTransform(puggo::translate(sceneObject.getTransform(), tX, tY, tZ));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
        SceneUtils::translate(child, tX, tY, tZ);
    }
    );
}

void SceneUtils::translate(shared_ptr<SceneObject>& sceneObject, const vec3& t) noexcept {
    sceneObject->setTransform(puggo::translate(sceneObject->getTransform(), t));

    for_each(
        sceneObject->getChildren().begin(),
        sceneObject->getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
            SceneUtils::translate(child, t);  
        }
    );
}

void SceneUtils::translate(shared_ptr<SceneObject>& sceneObject, const float& tX, const float& tY, const float& tZ) noexcept {
    sceneObject->setTransform(puggo::translate(sceneObject->getTransform(), tX, tY, tZ));

    for_each(
        sceneObject->getChildren().begin(),
        sceneObject->getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
            SceneUtils::translate(child, tX, tY, tZ);
        }
    );
}

void SceneUtils::rotate(SceneObject& sceneObject, const vec3& degrees) noexcept {
    sceneObject.setTransform(puggo::rotate(sceneObject.getTransform(), degrees));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
        SceneUtils::rotate(child, degrees);
    }
    );
}

void SceneUtils::rotate(SceneObject& sceneObject, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    sceneObject.setTransform(puggo::rotate(sceneObject.getTransform(), degreesX, degreesY, degreesZ));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
        SceneUtils::rotate(child, degreesX, degreesY, degreesZ);
    }
    );
}

void SceneUtils::rotate(shared_ptr<SceneObject>& sceneObject, const vec3& degrees) noexcept {
    sceneObject->setTransform(puggo::rotate(sceneObject->getTransform(), degrees));

    for_each(
        sceneObject->getChildren().begin(),
        sceneObject->getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
            SceneUtils::rotate(child, degrees);
        }
    );
}

void SceneUtils::rotate(shared_ptr<SceneObject>& sceneObject, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    sceneObject->setTransform(puggo::rotate(sceneObject->getTransform(), degreesX, degreesY, degreesZ));

    for_each(
        sceneObject->getChildren().begin(),
        sceneObject->getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
            SceneUtils::rotate(child, degreesX, degreesY, degreesZ);
        }
    );
}

void SceneUtils::orbit(SceneObject& sceneObject, const vec3& degrees) noexcept {
    sceneObject.setTransform(puggo::orbit(sceneObject.getTransform(), degrees));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
        SceneUtils::orbit(child, degrees);
    }
    );
}

void SceneUtils::orbit(SceneObject& sceneObject, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    sceneObject.setTransform(puggo::orbit(sceneObject.getTransform(), degreesX, degreesY, degreesZ));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
        SceneUtils::orbit(child, degreesX, degreesY, degreesZ);
    }
    );
}

void SceneUtils::orbit(shared_ptr<SceneObject>& sceneObject, const vec3& degrees) noexcept {
    sceneObject->setTransform(puggo::orbit(sceneObject->getTransform(), degrees));

    for_each(
        sceneObject->getChildren().begin(),
        sceneObject->getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
            SceneUtils::orbit(child, degrees);
        }
    );
}

void SceneUtils::orbit(shared_ptr<SceneObject>& sceneObject, const float& degreesX, const float& degreesY, const float& degreesZ) noexcept {
    sceneObject->setTransform(puggo::orbit(sceneObject->getTransform(), degreesX, degreesY, degreesZ));

    for_each(
        sceneObject->getChildren().begin(),
        sceneObject->getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
            SceneUtils::orbit(child, degreesX, degreesY, degreesZ);
        }
    );
}

void SceneUtils::scale(SceneObject& sceneObject, const vec3& s) noexcept {
    sceneObject.setTransform(puggo::scale(sceneObject.getTransform(), s));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
        SceneUtils::scale(child, s);
    }
    );
}

void SceneUtils::scale(SceneObject& sceneObject, const float& sX, const float& sY, const float& sZ) noexcept {
    sceneObject.setTransform(puggo::scale(sceneObject.getTransform(), sX, sY, sZ));

    for_each(
        sceneObject.getChildren().begin(),
        sceneObject.getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
        SceneUtils::scale(child, sX, sY, sZ);
    }
    );
}

void SceneUtils::scale(shared_ptr<SceneObject>& sceneObject, const vec3& s) noexcept {
    sceneObject->setTransform(puggo::scale(sceneObject->getTransform(), s));

    for_each(
        sceneObject->getChildren().begin(),
        sceneObject->getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
            SceneUtils::scale(child, s);
        }
    );
}

void SceneUtils::scale(shared_ptr<SceneObject>& sceneObject, const float& sX, const float& sY, const float& sZ) noexcept {
    sceneObject->setTransform(puggo::scale(sceneObject->getTransform(), sX, sY, sZ));

    for_each(
        sceneObject->getChildren().begin(),
        sceneObject->getChildren().end(),
        [&](shared_ptr<SceneObject>& child) {
            SceneUtils::scale(child, sX, sY, sZ);
        }
    );
}

void SceneUtils::appendChild(SceneObject& sceneObject, const shared_ptr<SceneObject>& newChild) noexcept {
    if (&sceneObject == newChild.get()) {
        return;
    }

    SceneUtils::removeChild(sceneObject, newChild);
    sceneObject.getChildren().push_back(newChild);

    SceneObject::ON_ADD_TO_SCENE_OBJECT payload = {
        &sceneObject,
        newChild
    };
    newChild->getManager().emit(payload);

    unsigned long id = 0;
    id = newChild->subscribeToOnAddToSceneObject([&sceneObject, &id](const SceneObject::ON_ADD_TO_SCENE_OBJECT& e) {
        if (e.parent != &sceneObject) {
            e.child->getManager().removeSubscription<SceneObject::ON_ADD_TO_SCENE_OBJECT>(id);
            SceneUtils::removeChild(sceneObject, e.child);
        }
    });
}

void SceneUtils::appendChild(shared_ptr<SceneObject>& sceneObject, const shared_ptr<SceneObject>& newChild) noexcept {
    SceneUtils::appendChild(*sceneObject.get(), newChild);
}

void SceneUtils::insertChild(SceneObject& sceneObject, const size_t& index, const shared_ptr<SceneObject>& newChild) noexcept {
    SceneUtils::removeChild(sceneObject, newChild);

    SceneObject::ON_ADD_TO_SCENE_OBJECT payload = {
        &sceneObject,
        newChild
    };

    unsigned long id = 0;
    function<void(const SceneObject::ON_ADD_TO_SCENE_OBJECT&)> onAddLambda = [&sceneObject, &id](const SceneObject::ON_ADD_TO_SCENE_OBJECT& e) {
        if (e.parent != &sceneObject) {
            e.child->getManager().removeSubscription<SceneObject::ON_ADD_TO_SCENE_OBJECT>(id);
            SceneUtils::removeChild(sceneObject, e.child);
        }
    };

    if (index >= sceneObject.getChildren().size()) {
        sceneObject.getChildren().insert(sceneObject.getChildren().begin() + index, newChild);
        newChild->getManager().emit(payload);
        id = newChild->subscribeToOnAddToSceneObject(onAddLambda);
    }
    else if (!sceneObject.getChildren().empty()) {
        sceneObject.getChildren().push_back(newChild);
        newChild->getManager().emit(payload);
        id = newChild->subscribeToOnAddToSceneObject(onAddLambda);
    }
}

void SceneUtils::insertChild(shared_ptr<SceneObject>& sceneObject, const size_t& index, const shared_ptr<SceneObject>& newChild) noexcept {
    SceneUtils::insertChild(*sceneObject.get(), index, newChild);
}

void SceneUtils::removeChild(SceneObject& sceneObject, const shared_ptr<SceneObject>& child) noexcept {
    for (auto it = sceneObject.getChildren().begin(); it != sceneObject.getChildren().end(); ++it) {
        if ((*it) == child) {
            sceneObject.getChildren().erase(it);

            SceneObject::ON_REMOVE_FROM_SCENE_OBJECT payload = {
                &sceneObject,
                child
            };
            child->getManager().emit(payload);
            break;
        }
    }
}

void SceneUtils::removeChild(SceneObject& sceneObject, const size_t& index) noexcept {
    if (index >= sceneObject.getChildren().size()) {
        vector<shared_ptr<SceneObject>>::iterator it = sceneObject.getChildren().begin() + index;
        const shared_ptr<SceneObject> child = (*it);
        sceneObject.getChildren().erase(it);
        SceneObject::ON_REMOVE_FROM_SCENE_OBJECT payload = {
                &sceneObject,
                child
        };
        child->getManager().emit(payload);
    }
}

void SceneUtils::removeChild(shared_ptr<SceneObject>& sceneObject, const shared_ptr<SceneObject>& child) noexcept {
    SceneUtils::removeChild(*sceneObject.get(), child);
}

void SceneUtils::removeChild(shared_ptr<SceneObject>& sceneObject, const size_t& index) noexcept {
    SceneUtils::removeChild(*sceneObject.get(), index);
}

void SceneUtils::replaceChild(SceneObject& sceneObject, const size_t& index, const shared_ptr<SceneObject>& newChild) noexcept {
    SceneUtils::removeChild(sceneObject, index);
    SceneUtils::insertChild(sceneObject, index, newChild);
}

void SceneUtils::replaceChild(shared_ptr<SceneObject>& sceneObject, const size_t& index, const shared_ptr<SceneObject>& newChild) noexcept {
    SceneUtils::replaceChild(*sceneObject.get(), index, newChild);
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

long SceneUtils::findChildIndex(shared_ptr<SceneObject>& sceneObject, const shared_ptr<SceneObject>& child) noexcept {
    return SceneUtils::findChildIndex(*sceneObject.get(), child);
}

string SceneUtils::createId(void) noexcept {
    return to_string(counter++);
}