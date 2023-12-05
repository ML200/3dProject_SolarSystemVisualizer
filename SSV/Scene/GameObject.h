//
// Created by m on 09.11.2023.
//

#ifndef OPENGL_TEMPLATE_GAMEOBJECT_H
#define OPENGL_TEMPLATE_GAMEOBJECT_H


#include <vector>
#include <iostream>
#include "glm/vec3.hpp"
#include "GL/glew.h"

class GameObject {
public:
    // List of vertices
    std::vector<GLfloat> vertexList;

    glm::vec3 color;
    // VBO ID for this object
    GLuint vertexBufferID;
    GLuint vertexArrayID;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    void setupVertexData() {
        // Generate and bind the VAO
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        // Generate and bind the VBO, then upload vertex data
        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(GLfloat), vertexList.data(), GL_STATIC_DRAW);

        // Set up vertex attribute pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Unbind the VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    //Change the transform vectors
    //Use this in the update function of the GameScene
    void translateObj(glm::vec3 vec);
    void rotateObj(glm::vec3 vec);
    void scaleObj(glm::vec3);

};


#endif //OPENGL_TEMPLATE_GAMEOBJECT_H
