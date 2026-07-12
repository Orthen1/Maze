
/** Includes **/
#include <iostream>



/**OpenGL dependecies **/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/** DEFINES **/
#define SCR_HEIGHT 600
#define SCR_WIDTH 600
#define WIREFRAME 0
#define TEST 1

/**  Prototypes **/

int initOpenGL();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

/**ERROR CODE**/
 enum ERR_CODE {
    GLFW_FAIL =  -1,
    SUCCESS = 0,
};

int main() {

    if (initOpenGL() != SUCCESS ) {
        return -1;
    }

    std::clog<< "\r GLFW Init Succes" << std::endl;

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Maze", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::clog<< "\r GLFW Window Creation Succes" << std::endl;
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Callback Functions for GLFW //

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLFW set Functions //

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwPollEvents();

    while (!glfwWindowShouldClose(window)) {
        //Input Process //
        ProcessInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();




    return 0;
}



int initOpenGL() {

    if (glfwInit() != GLFW_TRUE) {
        std::cerr<< "GLFW Init Failed\n" << std::endl;
        return GLFW_FAIL;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    return 0;



}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

}
