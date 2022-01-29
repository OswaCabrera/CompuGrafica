/*
* Laboratorio de Computación gráfica e interacción Humano-Computadora
* 02 - Introducción a OpenGL
* Based on: https://www.glfw.org/documentation.html
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include "Shader.h"
#include "Mesh.h"
#include "Triangle.h"
#include "MathFunction.h"

// Functions
bool Start();
bool Update();

// Definición de callbacks
        
void mouseCallback    (GLFWwindow* window, double xpos, double ypos);       // Mouse
void scrollCallback   (GLFWwindow* window, double xoffset, double yoffset); // Mouse movement
void keyboardCallback (GLFWwindow* window);                                 // Keyboard

void resizeCallback   (GLFWwindow* window, int width, int height);          // Window resize

using namespace std;

// Globals
GLFWwindow* window;
GLfloat  bgR, bgG, bgB, bgA;
Shader    * myShader;
Mesh      * mesh;

int main(void)
{
    if (!Start())
        return -1;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (!Update())
            break;
    }

    glfwTerminate();
    return 0;
}

bool Start() {

    /* Initialize the library GLFW */
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "You rocks!", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Define callbacks */
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Depth test activation
    glEnable(GL_DEPTH_TEST);

    // Background color initialization
    bgR = bgG = bgB = bgA = 0.0f;
    
    // Shaders compilation and linking
    myShader = new Shader("shaders/01-simple.vs", "shaders/01-simple.fs");
    if (myShader == nullptr) {
        cout << "Error creating shaders." << endl;
        return false;
    }

    // Init geometries
    Triangle tri(0.5f);
    MathFunction mfunc(0.5f);
    mesh = new Mesh(tri.vertices, tri.indices);
    if (mesh == nullptr) {
        cout << "Error creating mesh object." << endl;
        return false;
    }

    return true;
}

bool Update() {
    /* Process keyboard */
    keyboardCallback(window);
    
    /* Change the background Color */
    glClearColor(bgR, bgG, bgB, bgA);
    
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Draw objects */
    if (mesh != nullptr) {
        glUseProgram(myShader->ID);
        mesh->Draw(*myShader);
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();

    return true;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    // Do something when mouse actioned
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Do something when mouse moved
}

void keyboardCallback(GLFWwindow* window) {
    // Do something when key pressed

    // Exit action
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Standard keys for 3D movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

    }

    // User defined
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        bgR = 0.0f;
        bgG = 1.0f;
        bgB = 1.0f;
        bgA = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        bgR = 1.0f;
        bgG = 1.0f;
        bgB = 0.0f;
        bgA = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        bgR = 1.0f;
        bgG = 0.0f;
        bgB = 1.0f;
        bgA = 1.0f;
    }

    // Arrows
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

    }
}

void resizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}