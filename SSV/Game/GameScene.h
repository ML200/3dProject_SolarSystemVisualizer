//
// Created by m on 09.11.2023.
//

#ifndef OPENGL_TEMPLATE_GAMESCENE_H
#define OPENGL_TEMPLATE_GAMESCENE_H


#include <vector>
#include <memory>
#include "playground/GameObject.h"
#include "glfw3.h"

class GameScene {
public:
    std::vector<std::shared_ptr<GameObject>> coralObjects;
    std::vector<std::shared_ptr<GameObject>> bubbleObjects;
    std::vector<std::shared_ptr<GameObject>> gameObjects;

    std::vector<GLfloat> fishVertexList;
    std::vector<GLfloat> coralVertexList;
    std::vector<GLfloat> bubbleVertexList;

    //pointer to the window
    GLFWwindow* window;

    float coralSpawnTimer = 0.0f;
    float coralSpawnInterval = 2.0f;  // Time in seconds between coral spawns

    float bubbleSpawnTimer = 0.0f;
    float bubbleSpawnInterval = 5.0f;  // Time in seconds between coral spawns

    float globalTime = 0.0f;
    bool endGame = false;

    void awake();
    void update();

    void createFloor(float width, float height);

    void generateRandomCoral(float viewWidth);



    float playerVelocity = 0.0f;
    float playerAcceleration = 100.0f;  // Acceleration speed
    float maxPlayerSpeed = 50.0f;       // Maximum speed

    float speedMultiplier = 1.0f;       // Speed multiplier for the player

    void generateRandomBubble(float viewWidth);
};


#endif //OPENGL_TEMPLATE_GAMESCENE_H
