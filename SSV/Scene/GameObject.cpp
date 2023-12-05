//
// Created by m on 09.11.2023.
//

#include "GameObject.h"
#include "glm/detail/func_common.hpp"

// Translates the GameObject by a given vector
void GameObject::translateObj(glm::vec3 vec) {
    position += vec;
}

// Rotates the GameObject by a given vector (in degrees)
void GameObject::rotateObj(glm::vec3 vec) {
    rotation += vec;
    // Ensure rotation stays within 0-360 degrees to avoid overflow
    rotation = glm::mod(rotation, glm::vec3(360.0f));
}

// Scales the GameObject by a given vector
void GameObject::scaleObj(glm::vec3 vec) {
    scale *= vec;
}