//
// Created by m on 09.11.2023.
//

#include "MatrixUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 MatrixUtils::getTranslationMatrix(glm::vec3 translate) {
    // Create a translation matrix using glm::translate
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), translate);
    return translation;
}

glm::mat4 MatrixUtils::getRotationMatrix(glm::vec3 rotate) {
    // Assume rotate contains rotation angles in degrees for each axis
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Combine the rotation matrices. Note the multiplication order: Z * Y * X
    glm::mat4 rotation = rotationZ * rotationY * rotationX;
    return rotation;
}

glm::mat4 MatrixUtils::getScaleMatrix(glm::vec3 scale) {
    // Create a scaling matrix using glm::scale
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
    return scaling;
}

