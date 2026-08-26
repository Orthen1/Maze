
/** Includes **/
#include <iostream>
#include <random>
#include "Grid.h"


/**OpenGL dependecies **/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Mesh.h"
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
void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
unsigned int loadImage(const char* filename);
bool wallCollision(std::vector<Wall> walls);


/**ERROR CODE**/
 enum ERR_CODE {
    GLFW_FAIL =  -1,
    SUCCESS = 0,
};

/** GLOBAL VARIABLES **/

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH/2.0f;
float lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
std::vector<Wall> wallPos;



int main(int argc, char *args[]) {



    const glm::vec4 background_color(0.0,0.0,0.0,1.0);
    Grid grid(5,5);



    //*Shader load *//
    std::string vertes = "../Shaders/VertexShader.vs";
    std::string fragment = "../Shaders/FragmentShader.fs";

#if TEST
    std::string boxVs = "../Shaders/BoundingBox.vs";
    std::string boxFs = "../Shaders/BoundingBox.fs";
#endif




    float cubeVertices[] = {
        // BACK
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,

        // FRONT
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, 0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.5f, 1.0f, 1.0f,

         // LEFT
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, 0.5f, 1.0f, 1.0f,

        // RIGHT
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.5f, 0.0f, 1.0f,

        // BOTTOM
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,

        // TOP
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, 0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.5f, 1.0f, 1.0f
    };




    int indeces[] = {
        // BACK
        0, 1, 2,
        1, 3, 2,

        // FRONT
        4, 5, 6,
        5, 7, 6,

        // LEFT
        8, 9, 10,
        9, 11, 10,

        // RIGHT
        12, 13, 14,
        13, 15, 14,

        // BOTTOM
        16, 17, 18,
        17, 19, 18,

        // TOP
        20, 21, 22,
        21, 23, 22




    };

   // loadLayout("../maze.txt", wallPos);
    camera.Position = grid.generateWalls(wallPos);
#if TEST
    grid.printMaze();

    for (int y = 0; y < grid.getRows(); ++y) {
        for (int x = 0; x < grid.getCols(); ++x) {

            Tile* tile = grid.getTile(x, y);

            std::cout << "(" << x << "," << y << "): ";

            if (tile->direction & N)
                std::cout << "N ";

            if (tile->direction & S)
                std::cout << "S ";

            if (tile->direction & E)
                std::cout << "E ";

            if (tile->direction & W)
                std::cout << "W ";

            std::cout << '\n';
        }
    }



#endif






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

    // Callback Functions fo;r GLFW //

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLFW set Functions //
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, 640, 480);
    Shader shader = Shader(vertes.c_str(), fragment.c_str());
    shader.use();

#if TEST
    Shader Bbox = Shader(boxVs.c_str(),boxFs.c_str());
#endif


    // ** PARSING DATA TO GPU ** //

    unsigned int  VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);


    Mesh cubeMesh(cubeVertices,std::size(cubeVertices));


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_PROGRAM_POINT_SIZE);
    stbi_set_flip_vertically_on_load(true);

#if  WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPointSize(10.0f);
#else
 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif






    //** TEXTURE DEFINITION **//

    unsigned int texture = loadImage("../textures/shrub.jpg");

    shader.use();
    shader.setInt("texture", 0);

    glEnable(GL_DEPTH_TEST);

#if TEST
// defining binding box vertex
    std::vector<float> unitBoxVerts;
    std::vector<uint> boxLineIndices;


    unsigned int counter = 0;
    for (auto wall : wallPos){

        uint base = counter *8;

        std::vector<float> boxVerts = wall.box.boxToVertex();
        unitBoxVerts.insert(unitBoxVerts.end(),boxVerts.begin(),boxVerts.end());
        boxLineIndices.insert(boxLineIndices.end(), {

            base +0, base +1,
            base +1, base +2,
            base +2, base +3,
            base +3, base +0,

            base +4, base +5,
            base +5, base +6,
            base +6, base +7,
            base +7, base +4,

            base +0, base +4,
            base +1, base +5,
            base +2, base +6,
            base +3, base +7,

        });


    counter++;
    }

    unsigned int VAO_Collisions, VBO_Collisions, EBO_Collisions;
    glGenVertexArrays(1, &VAO_Collisions);
    glBindVertexArray(VAO_Collisions);

    glGenBuffers(1, &VBO_Collisions);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Collisions);
    glBufferData(GL_ARRAY_BUFFER, unitBoxVerts.size() * sizeof(float), unitBoxVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO_Collisions);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Collisions);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, boxLineIndices.size() * sizeof(uint), boxLineIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    Bbox.use();

#endif




    //**RENDER LOOP **//
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    #if TEST
        double fps = 1/ deltaTime;
       std::clog << "FPS:" << fps << std::endl;
        std::cout <<"is hitting a wall:" << wallCollision(wallPos) << std::endl;
    #endif
        //Input Process //
        ProcessInput(window);

        glClearColor(background_color.x,background_color.y,background_color.z,background_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Rendering Process //
        shader.use();

        // Matrix Space//
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f),float(SCR_WIDTH)/float(SCR_HEIGHT),0.1f,100.0f);
        shader.setMat4("projection",projection);

        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("view",view);

      /*  for (auto wall: wallPos){
            if (!wall.visible) {
                continue;
            }
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, wall.position);

            if (wall.vertical == true) {
                model = glm::scale(model,glm::vec3(wall.thickness,1.0f,1.0f));
            }else {
                model = glm::scale(model,glm::vec3(1.0f,1.0f,wall.thickness));
            }

            shader.setMat4("model",model);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



        }*/

        for (auto &tile: grid.getTiles()) {
            if (tile->wall != nullptr) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, tile->wall->position);

                if (tile->wall->vertical == true) {
                    model = glm::scale(model,glm::vec3(tile->wall->thickness,1.0f,1.0f));
                }else {
                    model = glm::scale(model,glm::vec3(1.0f,1.0f,tile->wall->thickness));
                }


                shader.setMat4("model",model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



            }

        }

#if TEST
        glDisable(GL_DEPTH_TEST);
        glLineWidth(2.0f);
        Bbox.use();
        Bbox.setMat4("view",view);
        Bbox.setMat4("projection",projection);
       for (auto wall : wallPos) {
          glm::mat4 model = glm::mat4(1.0f);
            Bbox.setMat4("model",model);
            glBindVertexArray(VAO_Collisions);
        glDrawElements(GL_LINES, boxLineIndices.size(), GL_UNSIGNED_INT, 0);

        }
        glEnable(GL_DEPTH_TEST);
#endif


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    unsigned int VBO = cubeMesh.GetVBO();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
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
        glm::vec3 oldPos = camera.Position;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if ( (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F))) {
        camera.fly= camera.fly == true ? false  : true;
        std::clog << camera.fly << std::endl;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W)) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S)) {
            camera.processKeyboard(BACKWARD, deltaTime);
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A)) {
            camera.processKeyboard(LEFT, deltaTime);
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D)) {
            camera.processKeyboard(RIGHT, deltaTime);

    }
    if (wallCollision(wallPos)) {
        camera.Position = oldPos;
    }

}


void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processMouse(xoffset, yoffset);
}



unsigned int loadImage(const char* filename) {
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);


    int width, height,nrChannels;

    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1 ) {
            format = GL_R;
        }else if (nrChannels == 3) {
            format = GL_RGB;
        }else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }else {
        std::cerr << "Failed to load texture" << filename << std::endl;
    }
    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);
    return texture;

}


bool wallCollision(std::vector<Wall> walls) {
    camera.updateCameraBoundingBox();
    for (auto wall : walls) {
            if (camera.bounding_box.intersec(wall.box)) {
                return  true;
            }

    }

    return false;

}



