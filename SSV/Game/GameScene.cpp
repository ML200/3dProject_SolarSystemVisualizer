//
// Created by m on 09.11.2023.
//

#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include "GameScene.h"
#include "Util/MatrixUtils.h"
#include "playground/Globals.h"
#include "glfw3.h"
#include "common/parse_stl.h"


float getRandomFloat(float min, float max) {
    // Obtain a seed from the system clock
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}


bool checkCollision3D(const GameObject& a, const GameObject& b) {
    // Calculate the sides of rect A
    float leftA = a.position.x;
    float rightA = a.position.x + a.scale.x;
    float topA = a.position.y;
    float bottomA = a.position.y + a.scale.y;
    float frontA = a.position.z;
    float backA = a.position.z + a.scale.z;

    // Calculate the sides of rect B
    float leftB = b.position.x;
    float rightB = b.position.x + b.scale.x;
    float topB = b.position.y;
    float bottomB = b.position.y + b.scale.y;
    float frontB = b.position.z;
    float backB = b.position.z + b.scale.z;

    // Check if any of the sides from A are outside of B
    if (leftA >= rightB || rightA <= leftB ||
        topA >= bottomB || bottomA <= topB ||
        frontA >= backB || backA <= frontB) {
        return false;
    }

    return true;
}



void GameScene::createFloor(float width, float height) {
    auto floorTile = std::make_shared<GameObject>();
    // Assuming origin at the bottom left
    floorTile->position = glm::vec3(0.0f, 0.0f, 0.0f);
    floorTile->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    floorTile->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    floorTile->vertexList = {
            0.0f, 0.0f, 0.0f,    // Bottom left
            -width, 0.0f, 0.0f,    // Bottom right
            0.0f, height, 0.0f,   // Top left
            0.0f, height, 0.0f,   // Top left
            -width, 0.0f, 0.0f,    // Bottom right
            -width, height, 0.0f   // Top right
    };
    floorTile->setupVertexData();
    gameObjects.push_back(floorTile);
}

void GameScene::generateRandomCoral(float viewWidth) {
    auto coral = std::make_shared<GameObject>();
    float coralWidth = getRandomFloat(1.0f, 10.0f);
    float coralHeight = getRandomFloat(5.0f, 30.0f);

    // Set coral properties
    coral->position = glm::vec3(-viewWidth, 5.0f, 0.0f);
    coral->scale = glm::vec3(coralWidth, coralHeight, 1.0f);

    coral->vertexList = coralVertexList;
    coral->color = glm::vec3(0.0f, 1.0f, 0.0f);
    coral->setupVertexData();
    coralObjects.push_back(coral);
    gameObjects.push_back(coral);
}


void GameScene::generateRandomBubble(float viewWidth) {
    auto bubble = std::make_shared<GameObject>();
    float bubbleSize = getRandomFloat(1.0f, 3.0f);
    float height = getRandomFloat(20.0f, 50.0f);

    // Set coral properties
    bubble->position = glm::vec3(-viewWidth, height, 0.0f);
    bubble->scale = glm::vec3(bubbleSize, bubbleSize, 1.0f);

    bubble->vertexList = bubbleVertexList;
    bubble->color = glm::vec3(0.0f, 0.0f, 1.0f);
    bubble->setupVertexData();
    bubbleObjects.push_back(bubble);
    gameObjects.push_back(bubble);
}



void GameScene::awake() {
    //Fill the vertex lists
    stl_data fish = parse_stl("fish.stl");
    fishVertexList = fish.toVertexList();

    stl_data coral = parse_stl("coral.stl");
    coralVertexList = coral.toVertexList();

    stl_data bubble = parse_stl("bubbles.stl");
    bubbleVertexList = bubble.toVertexList();

    /*stl_data bubble = parse_stl("bubble.stl");
    bubbleVertexList = bubble.toVertexList();*/



    //Set all the meshes used and their initial properties
    //Create an example cube
    auto cube = std::make_shared<GameObject>();
    cube->position = glm::vec3(-20.0f, 20.0f, 0.0f);
    cube->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    cube->scale = glm::vec3(4.0f, 4.0f, 4.0f);


    //Create the vertices for the cube
    cube->vertexList = fishVertexList;
    cube->color = glm::vec3(1.0f, 0.0f, 0.0f);
    cube->setupVertexData();

    gameObjects.push_back(cube);




    createFloor(1000000.0f, 1.0f);

}

void GameScene::update() {
    float movementSpeed = 10.0f;
    float viewWidth = 4.0f/3.0f * 50.0f;
    const float maxRotationAngle = 45.0f; // Max angle in degrees
    globalTime += deltaTime;


    float rotationAngle = (playerVelocity / maxPlayerSpeed) * maxRotationAngle;
    rotationAngle = std::max(-maxRotationAngle, std::min(rotationAngle, maxRotationAngle));
    gameObjects[0]->rotation.z=-rotationAngle; // Rotating around Z-axis

    // Handle player movement
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        // Accelerate player up, but only if not at the top boundary
        if (gameObjects[0]->position.y < 50.0f) {
            playerVelocity += playerAcceleration * deltaTime;
        }
    } else {
        // Accelerate player down, but only if not at the bottom boundary
        if (gameObjects[0]->position.y > 0) {
            playerVelocity -= playerAcceleration * deltaTime;
        }
    }

// Clamp player velocity to max speed
    playerVelocity = std::max(std::min(playerVelocity, maxPlayerSpeed), -maxPlayerSpeed);

// Update player position based on velocity
    gameObjects[0]->position.y += playerVelocity * deltaTime;



// Clamp player's Y position between 0 and screenHeight
    gameObjects[0]->position.y = std::max(0.0f, std::min(gameObjects[0]->position.y, 50.0f));

// Adjust velocity if player is at the border
    if (gameObjects[0]->position.y >= 50.0f || gameObjects[0]->position.y <= 0) {
        playerVelocity = 0.0f;
    }


    // Increment the coral spawn timer
    coralSpawnTimer += deltaTime;
    bubbleSpawnTimer += deltaTime;

    // Check if it's time to spawn a new coral
    if (coralSpawnTimer >= coralSpawnInterval) {
        generateRandomCoral(viewWidth);
        coralSpawnTimer = 0.0f;  // Reset the timer
        coralSpawnInterval = getRandomFloat(2.0f, 5.0f)/ (1.0f+(0.001f * globalTime *globalTime )); // Randomize next interval
        std::cout << "Spawned Coral" << std::endl;
    }


    // Check if it's time to spawn a new coral
    if (bubbleSpawnTimer >= bubbleSpawnInterval) {
        generateRandomBubble(viewWidth);
        bubbleSpawnTimer = 0.0f;  // Reset the timer
        bubbleSpawnInterval = getRandomFloat(3.0f, 10.0f) / (1.0f+(0.001f * globalTime *globalTime )); // Randomize next interval
        std::cout << "Spawned Bubble" << std::endl;
    }


    // Move corals and bubbles
    for (auto& coral : coralObjects) {
        coral->translateObj(glm::vec3((movementSpeed+(0.01f * globalTime *globalTime )) * deltaTime, 0.0f, 0.0f));
    }

    for (auto& bubble : bubbleObjects) {
        // Horizontal movement remains the same.
        float horizontalMovement = (movementSpeed+(0.01f * globalTime * globalTime)) * deltaTime;

        // Vertical movement based on sine wave and the bubble's unique strength.
        float verticalMovement = 2.0f  * sin(globalTime) *deltaTime;

        // Apply the translation with both horizontal and vertical components.
        bubble->translateObj(glm::vec3(horizontalMovement, verticalMovement, 0.0f));
        bubble->rotateObj(glm::vec3(0.0f, 0.0f, 10.5f*deltaTime));
    }

    // Check for 3D collisions
    for (auto& coral : coralObjects) {
        if (checkCollision3D(*gameObjects[0], *coral)) {
            std::cout << "3D Collision Detected!" << std::endl;
            endGame = true;

        }
    }

    // Check for 3D collisions
    for (auto& bubble : bubbleObjects) {
        if (checkCollision3D(*gameObjects[0], *bubble)) {
            std::cout << "3D Collision Detected!" << std::endl;
            endGame = true;

        }
    }
    // Remove or recycle corals
    coralObjects.erase(std::remove_if(coralObjects.begin(), coralObjects.end(),
                                      [viewWidth](const std::shared_ptr<GameObject>& coral) {
                                          return coral->position.x + coral->scale.x < -viewWidth;
                                      }), coralObjects.end());
}

