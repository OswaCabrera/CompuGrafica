/*
* Laboratorio de Computación Gráfica e Interacción Humano-Computadora
* 04 - Modelado Geométrico
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
#include "Triangle3D.h"
#include "Plane.h"
#include "Axis.h"
#include "Cube.h"
#include "Circle.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "MathFunction.h"
#include "Camera.h"

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
Shader    * myShader, *myOtherShader;
Mesh      *mesh, *meshAxis, *meshPlane, *meshCylinder;
int       screenWidth = 600, screenHeight = 600;

// Camera definition (XYZ position)
Camera camera(glm::vec3(8.0f, 8.0f, 8.0f));

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
    window = glfwCreateWindow(screenWidth, screenHeight, "You rocks!", NULL, NULL);
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
    myShader = new Shader("shaders/02-simplePVM.vs", "shaders/02-simplePVM.fs");
    if (myShader == nullptr) {
        cout << "Error creating shaders." << endl;
        return false;
    }

    myOtherShader = new Shader("shaders/02-simplePVM.vs", "shaders/02-simplePVM.fs");
    if (myOtherShader == nullptr) {
        cout << "Error creating shaders." << endl;
        return false;
    }

    // Init geometries
    //Triangle tri(0.5f);
    //MathFunction mfunc(0.5f);
    //Triangle3D tri3D(0.5f);
    // Plane plane(1.0f);
    // Cube cube(1.0f);
    // Circle circle(2.0f);
    Sphere sphere(1.0);
    mesh = new Mesh(sphere.vertices, sphere.indices);
    if (mesh == nullptr) {
        cout << "Error creating mesh object." << endl;
        return false;
    }

    Axis axis(10.0f);
    meshAxis = new Mesh(axis.vertices, axis.indices);
    if (meshAxis == nullptr) {
        cout << "Error creating mesh axis object." << endl;
        return false;
    }

    Plane plane(1.0f);
    meshPlane = new Mesh(plane.vertices, plane.indices);
    if (meshPlane == nullptr) {
        cout << "Error creating mesh plane object." << endl;
        return false;
    }

    Cylinder cylinder(1.0f);
    meshCylinder = new Mesh(cylinder.vertices, cylinder.indices);
    if (meshCylinder == nullptr) {
        cout << "Error creating mesh cylinder object." << endl;
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

        // Aplicamos transformaciones de proyección y cámara (si las hubiera)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
        //glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);
        myShader->setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(camera.Position, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        myShader->setMat4("view", view);

        // Aplicamos transformaciones del modelo
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,1.0f,0.0f)); 
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        myShader->setMat4("model", model);

        mesh->Draw(*myShader);

        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(0.0f, 2.5f, 0.0f));
        model2 = glm::rotate(model2, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
        myShader->setMat4("model", model2);

        mesh->Draw(*myShader);
    }

    glUseProgram(0);

    if (meshAxis != nullptr) {
        glUseProgram(myOtherShader->ID);

        // Aplicamos transformaciones de proyección y cámara (si las hubiera)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
        myOtherShader->setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(camera.Position, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        myOtherShader->setMat4("view", view);

        // Aplicamos transformaciones del modelo
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        myOtherShader->setMat4("model", model);

        meshAxis->Draw(*myOtherShader);

    }

    glUseProgram(0);

    if (meshPlane != nullptr) {
        glUseProgram(myOtherShader->ID);

        // Aplicamos transformaciones de proyección y cámara (si las hubiera)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
        myOtherShader->setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(camera.Position, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        myOtherShader->setMat4("view", view);

        // Aplicamos transformaciones del modelo
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
        myOtherShader->setMat4("model", model);

        meshPlane->Draw(*myOtherShader);

    }

    glUseProgram(0);

    if (meshCylinder != nullptr) {
        glUseProgram(myOtherShader->ID);

        // Aplicamos transformaciones de proyección y cámara (si las hubiera)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
        myOtherShader->setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(camera.Position, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        myOtherShader->setMat4("view", view);

        // Aplicamos transformaciones del modelo
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 3.3f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        myOtherShader->setMat4("model", model);

        meshCylinder->Draw(*myOtherShader);

        // Aplicamos transformaciones del modelo
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(2.0f, 1.0f, -2.0f));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 1.0f));
        myOtherShader->setMat4("model", model2);

        meshCylinder->Draw(*myOtherShader);

    }

    glUseProgram(0);

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

    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {

    }

    // Special keys
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {

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

    // Functions
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {

    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {

    }
}

void resizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}