
#include "main.h"

int main(int argc, char* args[]) {
    // TODO help function
    if (argc >= 2) {
        grid.setCols((atoi(args[0])));
        grid.setRows((atoi(args[1])));
    }

    grid.Init();

    constexpr glm::vec4 background_color(0.0, 0.0, 0.0, 1.0);

    //*Shader load *//
    std::string mazeVertex = "../Shaders/VertexShader.vs";
    std::string mazeFragment = "../Shaders/FragmentShader.fs";
    std::string lightVertex = "../Shaders/floatingLight.vs";
    std::string lightFragment = "../Shaders/floatingLight.fs";
    std::string pariclesFragment = "../Shaders/Particles.fs";
    std::string particlesVertex = "../Shaders/Particles.vs";

#if BOUNDING_BOX
    std::string boxVs = "../Shaders/BoundingBox.vs";
    std::string boxFs = "../Shaders/BoundingBox.fs";
#endif
    // clang-format off
    std::vector<float> cubeVertices = {
        // BACK
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        // FRONT
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        // LEFT
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        // RIGHT
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        // BOTTOM
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        // TOP
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    };

    //clang-format on

    std::vector<uint> indeces = {
        // BACK
        0, 1, 2, 1, 3, 2,

        // FRONT
        4, 5, 6, 5, 7, 6,

        // LEFT
        8, 9, 10, 9, 11, 10,

        // RIGHT
        12, 13, 14, 13, 15, 14,

        // BOTTOM
        16, 17, 18, 17, 19, 18,

        // TOP
        20, 21, 22, 21, 23, 22

    };

    if (initOpenGL() != SUCCESS) {
        return -1;
    }

    std::clog << "\r GLFW Init Sucess" << std::endl;

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Maze", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::clog << "\r GLFW Window Creation Sucess" << std::endl;
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Callback Functions for GLFW //

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLFW set Functions //
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, 640, 480);



    // **  Shaders ** //
    Shader mazeShader = Shader(mazeVertex.c_str(), mazeFragment.c_str());
    mazeShader.use();
    Shader lightShader = Shader(lightVertex.c_str(), lightFragment.c_str());
    lightShader.use();
    Shader particelShader = Shader(particlesVertex.c_str(),pariclesFragment.c_str());

#if BOUNDING_BOX
    Shader Bbox = Shader(boxVs.c_str(), boxFs.c_str());
    Bbox.use();
#endif


    glEnable(GL_PROGRAM_POINT_SIZE);
    stbi_set_flip_vertically_on_load(true);


    //** Create Grid **//
    camera.Position = grid.generateWalls();
    glm::vec3 exitPos = grid.getExit();
    if (exitPos == glm::vec3(-1)) {
        std::cerr << "Grid creation failed no exit found" << std::endl;
        return GRID_FAILED;
    }
    //** Setting Particles **//
    ParticlesPool particles(MAX_PARTICLES);


    //**Wall Batching **//

    std::vector<float> mazeWallVerteces;
    std::vector<uint> mazeWallIndeces;

    unsigned int wallCounter = 0;


    for (auto& wall: grid.getWalls()) {
        if (!wall->visible) {
            continue;
        }
        uint wallBase = wallCounter * 24;
        for (int i = 0 ; i < cubeVertices.size(); i+= 8) {
            mazeWallVerteces.push_back(cubeVertices[i+0]+wall->position.x);
            mazeWallVerteces.push_back(cubeVertices[i+1]+wall->position.y);
            mazeWallVerteces.push_back(cubeVertices[i+2]+wall->position.z);
            mazeWallVerteces.push_back(cubeVertices[i+3]);
            mazeWallVerteces.push_back(cubeVertices[i+4]);
            mazeWallVerteces.push_back(cubeVertices[i+5]);
            mazeWallVerteces.push_back(cubeVertices[i+6]);
            mazeWallVerteces.push_back(cubeVertices[i+7]);
        }
        for (auto &index : indeces) {
            mazeWallIndeces.push_back(wallBase+index);
        }
        wallCounter++;
    }

    //** Floor and Cieling Batching **//

    std::vector<float> floorVerts, ceilingVerts;
    std::vector<uint> floorIdx, ceilingIdx;
    uint counter = 0;
    for (auto &cell: grid.getCells()) {
        if (cell->isWall && (cell->worldPos != exitPos)) {
            continue;
        }
        glm::vec3 p = cell->worldPos;
        float fVerts[] = {
            p.x - 0.5f, p.y - 0.51f, p.z - 0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            p.x + 0.5f, p.y - 0.51f, p.z - 0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            p.x + 0.5f, p.y - 0.51f, p.z + 0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            p.x - 0.5f, p.y - 0.51f, p.z + 0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        };
        floorVerts.insert(floorVerts.end(), std::begin(fVerts), std::end(fVerts));
        uint index= counter * 4;
        floorIdx.insert(floorIdx.end(), {index+0, index+1, index+2, index+0, index+2, index+3});

        float cVerts[] = {
            p.x - 0.5f, p.y + 0.51f, p.z - 0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            p.x + 0.5f, p.y + 0.51f, p.z - 0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            p.x + 0.5f, p.y + 0.51f, p.z + 0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
            p.x - 0.5f, p.y + 0.51f, p.z + 0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
        };
        ceilingVerts.insert(ceilingVerts.end(), std::begin(cVerts), std::end(cVerts));
        ceilingIdx.insert(ceilingIdx.end(), {index+0, index+1, index+2, index+0, index+2, index+3});
        counter++;
    }



#if BOUNDING_BOX
    // defining binding box vertex
    std::vector<float> unitBoxVerts;
    std::vector<uint> boxLineIndices;

    counter = 0;
    for (auto& wall : grid.getWalls()) {
        uint base = counter * 8;

        std::vector<float> boxVerts = wall->box.boxToVertex();
        unitBoxVerts.insert(unitBoxVerts.end(), boxVerts.begin(), boxVerts.end());
        boxLineIndices.insert(
            boxLineIndices.end(), {

                                      base + 0, base + 1, base + 1, base + 2, base + 2, base + 3, base + 3, base + 0,

                                      base + 4, base + 5, base + 5, base + 6, base + 6, base + 7, base + 7, base + 4,

                                      base + 0, base + 4, base + 1, base + 5, base + 2, base + 6, base + 3, base + 7,

                                  }
        );

        counter++;
    }
#endif

#if GRID
    counter = 0;
    std::vector<float> gridVerts;
    std::vector<uint> tileIndices;
    std::vector<uint> wallIndices;
    for (auto& cell : grid.getCells()) {
        std::vector<float> tileVerts = {
            cell->worldPos.x - 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z - 0.5f, cell->worldPos.x + 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z - 0.5f,
            cell->worldPos.x + 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z + 0.5f, cell->worldPos.x - 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z + 0.5f,
        };
        gridVerts.insert(gridVerts.end(), tileVerts.begin(), tileVerts.end());
        uint base = counter * 4;

        if (cell->isWall) {
            wallIndices.insert(
                wallIndices.end(), {
                                       base + 0,
                                       base + 1,
                                       base + 2,
                                       base + 0,
                                       base + 2,
                                       base + 3,
                                   }
            );
        } else {
            tileIndices.insert(
                tileIndices.end(), {
                                       base + 0,
                                       base + 1,
                                       base + 1,
                                       base + 2,
                                       base + 2,
                                       base + 3,
                                       base + 3,
                                       base + 0,
                                   }
            );
        }
        counter++;
    }

    //**Binding Box Mesh**//
    Mesh bondingBoxMesh(unitBoxVerts, std::size(unitBoxVerts), {{0, 3}}, boxLineIndices, std::size(boxLineIndices));

    Bbox.use();

    //**TILE MESH **//
    Mesh gridMesh(gridVerts, std::size(gridVerts), {{0, 3}}, tileIndices, std::size(tileIndices));
    Mesh wallMesh(gridVerts, std::size(gridVerts), {{0, 3}}, wallIndices, std::size(wallIndices));
    Bbox.use();

#endif

    // ** Meshes ** //

    Mesh cubeMesh(mazeWallVerteces, std::size(mazeWallVerteces), {{0, 3}, {1, 3}, {2, 2}}, mazeWallIndeces, std::size(mazeWallIndeces));
    Mesh lightCube(cubeVertices,std::size(cubeVertices),{{0,3}, {1,3}, {2, 2}},indeces,std::size(indeces));
    Mesh floorMesh(floorVerts, std::size(floorVerts),{{0,3},{1,3},{2,2}},floorIdx,std::size(floorIdx));
    Mesh ceilingMesh(ceilingVerts, std::size(ceilingVerts),{{0,3},{1,3},{2,2}},ceilingIdx,std::size(ceilingIdx));
#if TEST
    grid.printMaze();

#endif
    float minSpacing = 4.0f;
    //**LIGHT INIT**//
    std::vector<Light> lights;
    int numOfLights = 0;
    for (auto& cell: grid.getCells()) {
        std::mt19937 random(std::random_device{}());
        std::uniform_real_distribution<float> spawnChance(0.0f,1.0f);
        if (numOfLights == MAX_POINT_LIGHTS) {
            break;
        }
        if ((spawnChance(random) >= 0.15f) && cell->isWall == false) {

            glm::vec3 position(cell->worldPos.x,cell->worldPos.y , cell->worldPos.z);
            bool tooClose = false;
            for (auto &exist : lights) {
                if (glm::distance(exist.position, position) < minSpacing) {
                    tooClose = true;
                    break;
                }
            }
            if (!tooClose) {
                glm::vec3 color(0.95f,0.48f,0.01f);

                float constant = 1.0f;
                float linear = 0.09f;
                float quadratic = 0.032f;
                lights.insert(lights.end(),{position,constant,linear,quadratic,color});

                numOfLights++;
            }
        }
    }

    //** TEXTURE DEFINITION  **//

    unsigned int wallTexture = loadImage("../textures/wall.jpg");
    unsigned int wallSpecularTexture = loadImage("../textures/stone_specular.png");
//** MATERIAL, LIGHT, DEFINITION **//

    mazeShader.use();
    mazeShader.setInt("material.diffusionMap", 0);
    mazeShader.setInt("material.specularMap", 1);
    mazeShader.setFloat("material.shininess",64.0f);


    unsigned int floorTexture = loadImage("../textures/floor.jpg");
    unsigned int floorSpecular = loadImage("../textures/floor_specular.png");
    mazeShader.use();

    mazeShader.setVec3("dirLight.direction",{-0.05f,-1.0f,-0.05f});
    mazeShader.setVec3("dirLight.ambient",{0.1f,0.1f,0.12f});
    mazeShader.setVec3("dirLight.diffuse",{0.18f,0.18f,0.2f});
    mazeShader.setVec3("dirLight.specular",{0.05f,0.05f,0.05f});

    mazeShader.setInt("numOfPointLights",numOfLights);
    for (int i = 0; i < lights.size() ; i++) {
        std::string base = "light[" + std::to_string(i) + "]";
        mazeShader.setVec3(base+".position",lights[i].position);
        mazeShader.setVec3(base+".ambient",lights[i].color*0.1f);
        mazeShader.setVec3(base+".diffuse",lights[i].color);
        mazeShader.setVec3(base+".specular",lights[i].color);
        mazeShader.setFloat(base+".constant",lights[i].constant);
        mazeShader.setFloat(base+".linear",lights[i].linear);
        mazeShader.setFloat(base+".quadratic",lights[i].quadratic);

    }


    glEnable(GL_DEPTH_TEST);

    //**RENDER LOOP **//
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
#if FPS
        double fps = 1 / deltaTime;
        std::clog << "FPS:" << fps << std::endl;
#endif
#if BOUNDING_BOX
        std::clog << "is hitting a wall:" << wallCollision(grid) << std::endl;
#endif

        // Input Process //
        ProcessInput(window);

        glClearColor(background_color.x, background_color.y, background_color.z, background_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,wallSpecularTexture);

        // Rendering Process //
        mazeShader.use();

        // Matrix Space//
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
        mazeShader.setMat4("projection", projection);

        glm::mat4 view = camera.getViewMatrix();
        mazeShader.setMat4("view", view);


            glm::mat4 model = glm::mat4(1.0f);
            mazeShader.setMat4("model", model);
            glBindVertexArray(cubeMesh.getVAO());
            glDrawElements(GL_TRIANGLES, mazeWallIndeces.size(), GL_UNSIGNED_INT, 0);
        mazeShader.use();
        mazeShader.setInt("material.diffusionMap", 0);
        mazeShader.setInt("material.specularMap", 1);
        mazeShader.setFloat("material.shininess",32.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);


            glBindVertexArray(floorMesh.getVAO());
            glDrawElements(GL_TRIANGLES, floorIdx.size(), GL_UNSIGNED_INT, 0);
            mazeShader.setMat4("model", model);

        glBindVertexArray(ceilingMesh.getVAO());
        glDrawElements(GL_TRIANGLES, ceilingIdx.size(), GL_UNSIGNED_INT, 0);

        lightShader.use();
        lightShader.setMat4("projection",projection);
        lightShader.setMat4("view",view);
        for (auto &light:lights) {
            glm::mat4 model  = glm::mat4(1.0);
            model = glm::translate(model,{light.position.x,light.position.y + 0.3, light.position.z});
            model = glm::scale(model,{0.1f,0.1f,0.1f});
            lightShader.setMat4("model",model);
            glBindVertexArray(lightCube.getVAO());
            glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);


        }
        if (reachedExit) {
            particelShader.use();
            particelShader.setMat4("projection",projection);
            particelShader.setMat4("view",view);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,glm::vec3(0.0f,0.0f,0.0f));
            particelShader.setMat4("model",glm::mat4(1.0f));

            particles.Pour({exitPos.x,exitPos.y +0.5f,exitPos.z},12000,deltaTime);
            particles.Update(deltaTime);
            particles.Render();
        }

#if BOUNDING_BOX
        glDisable(GL_DEPTH_TEST);
        glLineWidth(2.0f);
        Bbox.use();
        Bbox.setMat4("view", view);
        Bbox.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        Bbox.setMat4("model", model);
        glBindVertexArray(bondingBoxMesh.getVAO());
        glDrawElements(GL_LINES, boxLineIndices.size(), GL_UNSIGNED_INT, 0);
#endif

#if GRID
        glLineWidth(2.0f);
        Bbox.use();
        Bbox.setMat4("view", view);
        Bbox.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        Bbox.setMat4("model", model);
        glBindVertexArray(gridMesh.getVAO());
        glDrawElements(GL_LINES, tileIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(wallMesh.getVAO());
        glDrawElements(GL_TRIANGLES, wallIndices.size(), GL_UNSIGNED_INT, 0);

        glEnable(GL_DEPTH_TEST);
#endif

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

int initOpenGL() {
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "GLFW Init Failed\n" << std::endl;
        return GLFW_FAIL;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

void ProcessInput(GLFWwindow* window) {
    glm::vec3 oldPos = camera.Position;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if ((GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F))) {
        camera.fly = camera.fly == true ? false : true;
        camera.collisionsOn = camera.collisionsOn == true ? false : true;
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
    if (wallCollision(grid)) {
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
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    int width, height, nrChannels;

    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_R;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    } else {
        std::cerr << "Failed to load texture" << filename << std::endl;
    }
    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);
    return texture;
}

bool wallCollision(Grid& grid) {
    if (!camera.collisionsOn) {
        return false;
    }
    camera.updateCameraBoundingBox();
    for (const auto& wall : grid.getWalls()) {
        if (camera.bounding_box.intersect(wall->box)) {
            if (!wall->visible) {
                std::clog << "You have reached the end" << std::endl;
                reachedExit = true;

            }
            return true;
        }
    }

    return false;
}
