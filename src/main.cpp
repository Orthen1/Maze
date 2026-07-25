
/** Includes **/
#include <iostream>



/**OpenGL dependecies **/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "stb_image.h"

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

    std::string vertes = "../Shaders/VertexShader.vs";
    std::string fragment = "../Shaders/FragmentShader.fs";


    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f,  0.0f, 0.0f,
    };

    int indeces[] = {
        0,1,2,
        1,2,3,
    };

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
    glViewport(0, 0, 640, 480);
    Shader shader = Shader(vertes.c_str(), fragment.c_str());
    shader.use();

    // ** PARSING DATA TO GPU ** //

    unsigned int  VAO, VBO, EBO;
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_PROGRAM_POINT_SIZE);

#if  WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPointSize(10.0f);
#else
 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    //** TEXTURE DEFINITION **//

    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    int width, height,nrChannels;

    unsigned char *data = stbi_load("../textures/shrub.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);

    shader.use();
    shader.setInt("texture", 0);


    //**RENDER LOOP **//

    while (!glfwWindowShouldClose(window)) {
        //Input Process //
        ProcessInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
