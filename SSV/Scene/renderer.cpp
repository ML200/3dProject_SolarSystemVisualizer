#include "renderer.h"
#include "GameObject.h"
#include "Game/GameScene.h"
#include "Globals.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <iostream>
#include <chrono>


void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam) {
    // Print debug messages
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
              << " type = " << type
              << ", severity = " << severity
              << ", message = " << message << "\n";
}



glm::mat4 initializeMVPTransformation(std::shared_ptr<GameObject> gameObject)
{
    // Get a handle for our "MVP" uniform
    GLuint MatrixIDnew = glGetUniformLocation(programID, "MVP");
    MatrixID = MatrixIDnew;

    // Assuming Projection and View matrices are the same for all objects and pre-computed:
    glm::mat4 Projection = glm::ortho(0.0f, (4.0f/3.0f)*50.0f, 0.0f, 50.0f, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, -3), // Camera is at (4,3,-3), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : transformation matrix for the GameObject
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::translate(Model, gameObject->position);
    Model = glm::rotate(Model, glm::radians(gameObject->rotation.x), glm::vec3(1, 0, 0));
    Model = glm::rotate(Model, glm::radians(gameObject->rotation.y), glm::vec3(0, 1, 0));
    Model = glm::rotate(Model, glm::radians(gameObject->rotation.z), glm::vec3(0, 0, 1));
    Model = glm::scale(Model, gameObject->scale);

    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

    return MVP;
}



void updateAnimationLoop(std::vector<std::shared_ptr<GameObject>> gameObjects)
{

    auto startTime = std::chrono::system_clock::now();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    for (auto& gameObject : gameObjects) {
        // Compute the MVP matrix from gameObject parameters
        glm::mat4 MVP = initializeMVPTransformation(gameObject);
        GLint colorLocation = glGetUniformLocation(programID, "inputColor");

        glBindVertexArray(gameObject->vertexArrayID);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniform3f(colorLocation, gameObject->color.x, gameObject->color.y, gameObject->color.z);


        // Draw the object
        glDrawArrays(GL_TRIANGLES, 0, gameObject->vertexList.size()); // Drawing the object

        glBindVertexArray(0);
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    auto endTime = std::chrono::system_clock::now();

    std::chrono::duration<float> elapsed = endTime - startTime;
    deltaTime = elapsed.count();  // deltaTime in seconds
}





int main( void )
{

    //Initialize window
    bool windowInitialized = initializeWindow();
    if (!windowInitialized) return -1;


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");


    GameScene gameScene;
    gameScene.window = window;
    gameScene.awake();

    glEnable(GL_DEPTH_TEST);

    //start animation loop until escape key is pressed
    do{  
        gameScene.update();
        updateAnimationLoop(gameScene.gameObjects);
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 && !gameScene.endGame  );


    //Cleanup and close window
    cleanupVertexbuffer();
    glDeleteProgram(programID);
    closeWindow();

    return 0;
}




bool initializeWindow()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return false;
    }



    glfwWindowHint(GLFW_SAMPLES, 4);
    // Request an OpenGL debug context.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // Request debug context

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Demo: Cube", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return false;
    }

    // Enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    return true;
}

/*bool initializeMVPTransformation()
{
    // Get a handle for our "MVP" uniform
    GLuint MatrixIDnew = glGetUniformLocation(programID, "MVP");
    MatrixID = MatrixIDnew;


    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    //glm::mat4 Projection = glm::frustum(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
    glm::mat4 Projection = glm::ortho(0.0f, 4.0f/3.0f*5.0f, 0.0f, 5.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, -3), // Camera is at (4,3,-3), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);

    Model = glm::rotate(Model, curr_angle, glm::vec3(0.0f, 1.0f, 1.0f));

    glm::mat4 transformation;//additional transformation for the model
    transformation[0][0] = 1.0; transformation[1][0] = 0.0; transformation[2][0] = 0.0; transformation[3][0] = curr_x;
    transformation[0][1] = 0.0; transformation[1][1] = 1.0; transformation[2][1] = 0.0; transformation[3][1] = curr_y;
    transformation[0][2] = 0.0; transformation[1][2] = 0.0; transformation[2][2] = 1.0; transformation[3][2] = 0.0;
    transformation[0][3] = 0.0; transformation[1][3] = 0.0; transformation[2][3] = 0.0; transformation[3][3] = 1.0;

    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = Projection * View * Model * transformation; // Remember, matrix multiplication is the other way around


    return true;

}*/



bool initializeVertexbuffer()
{
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    vertexbuffer_size = 12;
    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f,-1.0f,-1.0f, // triangle 1 : begin
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, // triangle 1 : end
            1.0f, 1.0f,-1.0f, // triangle 2 : begin
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f, // triangle 2 : end
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
    };

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    return true;
}

bool cleanupVertexbuffer()
{
    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    return true;
}

bool closeWindow()
{
    glfwTerminate();
    return true;
}

