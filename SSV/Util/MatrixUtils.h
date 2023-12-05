//
// Created by m on 09.11.2023.
//

#ifndef OPENGL_TEMPLATE_MATRIXUTILS_H
#define OPENGL_TEMPLATE_MATRIXUTILS_H


#include "glm/detail/type_mat.hpp"
#include "glm/vec3.hpp"

//Class for creating matrices
class MatrixUtils {
public:
    glm::mat4 getTranslationMatrix(glm::vec3 translate);
    glm::mat4 getRotationMatrix(glm::vec3 rotate);
    glm::mat4 getScaleMatrix(glm::vec3 scale);
};


#endif //OPENGL_TEMPLATE_MATRIXUTILS_H
