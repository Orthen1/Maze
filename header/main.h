//
// Created by teo on 27. 8. 2026.
//

#ifndef MAZE_MAIN_H
#define MAZE_MAIN_H
#pragma once
/** Includes **/
#include <iostream>
#include <random>

// clang-format off
/**OpenGL dependecies **/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Grid.h"
#include "Light.h"
#include "Mesh.h"
#include "ParticlesPool.h"
#include "Shader.h"
#include "stb_image.h"
#include "Batch.h"
// clang-format on
/** DEFINES **/
#define SCR_HEIGHT    480
#define SCR_WIDTH     640
#define MAX_PARTICLES 12000
#define TEST          0
#define FPS           1

/**  Prototypes **/

int initOpenGL();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
unsigned int loadImage(const char* filename);
bool wallCollision(Grid& grid);

/**ERROR CODE**/
enum ERR_CODE {
    TO_MANY_PARAMETERS = -3,
    GRID_FAILED = -2,
    GLFW_FAIL = -1,
    SUCCESS = 0,
};
/**FLAGS**/
bool debugBox = false;   // Runtime toggle for bounding-box debug rendering
bool debugGrid = false;  // Runtime toggle for grid debug rendering

/** GLOBAL VARIABLES **/

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Grid grid(5, 5);

bool reachedExit = false;
#endif  // MAZE_MAIN_H
