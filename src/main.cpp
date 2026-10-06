
#include "main.h"

    int main(int argc, char* args[]) {
        if (argc == 3) {
            grid.setCols((atoi(args[1])));
        grid.setRows((atoi(args[2])));
    } else if (argc >= 3 || argc == 2) {
        std::cout << "Unexprected number of parameters" << std::endl;
        return TO_MANY_PARAMETERS;
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
    std::string boxVs = "../Shaders/BoundingBox.vs";
    std::string boxFs = "../Shaders/BoundingBox.fs";

    // clang-format off
    std::vector<Vertex> cubeVertices = {
        // BACK
        Vertex({-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}),
        Vertex({ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}),
        Vertex({-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}),
        Vertex({ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}),
        // FRONT
        Vertex({-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}),
        Vertex({ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}),
        Vertex({-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}),
        Vertex({ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}),
        // LEFT
        Vertex({-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}),
        Vertex({-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}),
        Vertex({-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}),
        Vertex({-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}),
        // RIGHT
        Vertex({ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}),
        Vertex({ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}),
        Vertex({ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}),
        Vertex({ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}),
        // BOTTOM
        Vertex({-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}),
        Vertex({ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}),
        Vertex({-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}),
        Vertex({ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}),
        // TOP
        Vertex({-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}),
        Vertex({ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}),
        Vertex({-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}),
        Vertex({ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}),
    };

    //clang-format on

    const std::vector<unsigned int> indeces = {
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);



    // **  Shaders ** //
    Shader mazeShader = Shader(mazeVertex.c_str(), mazeFragment.c_str());
    mazeShader.use();
    Shader lightShader = Shader(lightVertex.c_str(), lightFragment.c_str());
    lightShader.use();
    Shader particelShader = Shader(particlesVertex.c_str(),pariclesFragment.c_str());
    Shader Bbox = Shader(boxVs.c_str(), boxFs.c_str());
    Bbox.use();





    //** Create Grid **//
    camera.Position = grid.generateWalls();
    glm::vec3 exitPos = grid.getExit();
    if (exitPos == glm::vec3(-1)) {
        std::cerr << "Grid creation failed no exit found" << std::endl;
        return GRID_FAILED;
    }
    //** Setting Particles **//
    ParticlesPool particles(MAX_PARTICLES);
    //** Batching Geomtery **/
    BatchGeometry mazeWallsBatch = createWallBatch(grid,cubeVertices,indeces);
    BatchGeometry floorBatch =createFloorBatch(grid,exitPos) ;
    BatchGeometry ceilingBatch = createCeilingBatch(grid,exitPos);

    DebugGeometry boundsGemetry = buildBoundingBoxDebugGeometry(grid);
    Mesh bondingBoxMesh(boundsGemetry.vertcies,{},boundsGemetry.indeces);

    // Build per-cell debug geometry: solid quads for wall footprints,
    // wireframe outlines for open tiles — used when debugGrid is toggled on.
    unsigned int counter = 0;
    std::vector<Vertex> gridVerts;
    std::vector<unsigned int> tileIndices;
    std::vector<unsigned int> wallIndices;
    for (auto& cell : grid.getCells()) {
        std::vector<Vertex> tileVerts = {
            Vertex({cell->worldPos.x - 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z - 0.5f}),
            Vertex({cell->worldPos.x + 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z - 0.5f}),
            Vertex({cell->worldPos.x - 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z + 0.5f}),
           Vertex({cell->worldPos.x + 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z + 0.5f})
        };

        gridVerts.insert(gridVerts.end(), tileVerts.begin(), tileVerts.end());
        unsigned int base = counter * 4;

        if (cell->isWall) {
            wallIndices.insert(
                wallIndices.end(), {
                                       base + 0,
                                       base + 2,
                                       base + 1,
                                       base + 2,
                                       base + 3,
                                       base + 1,
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

        //** TEXTURE DEFINITION  **//

        unsigned int wallTexture = loadImage("../textures/wall.jpg");
        unsigned int wallSpecularTexture = loadImage("../textures/stone_specular.png");
        std::vector<Texture> wallTextures = {{wallTexture,"texture_diffuse"}, {wallSpecularTexture, "texture_specular"}} ;

        unsigned int floorTexture = loadImage("../textures/floor.jpg");
        unsigned int floorSpecular = loadImage("../textures/floor_specular.png");
        std::vector<Texture> floorTextures = {{floorTexture,"texture_diffuse"}, {floorSpecular,"texture_specular"}};

    //** CREATING MESHES **//
    Mesh gridMesh(gridVerts,{}, tileIndices );
    Mesh wallMesh(gridVerts,{}, wallIndices);
    Bbox.use();
    Mesh cubeMesh(mazeWallsBatch.vertcies,wallTextures, mazeWallsBatch.indeces);
    Mesh lightCube(cubeVertices,{},indeces);
    Mesh floorMesh(floorBatch.vertcies,floorTextures,floorBatch.indeces);
    Mesh ceilingMesh(ceilingBatch.vertcies, floorTextures,ceilingBatch.indeces);
#if TEST
    grid.printMaze();

#endif
    float minSpacing = 4.0f;
    //**LIGHT INIT**//
    std::vector<Light> lights = Light::generatePointLight(grid,4.0f);



    //** MATERIAL, LIGHT, DEFINITION **//
;
    mazeShader.use();


    mazeShader.setVec3("dirLight.direction",{-0.05f,-1.0f,-0.05f});
    mazeShader.setVec3("dirLight.ambient",{0.1f,0.1f,0.12f});
    mazeShader.setVec3("dirLight.diffuse",{0.18f,0.18f,0.2f});
    mazeShader.setVec3("dirLight.specular",{0.05f,0.05f,0.05f});

    mazeShader.setInt("numOfPointLights",lights.size());
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
        if (debugBox) {
            std::clog << "is hitting a wall:" << wallCollision(grid) << std::endl;
        }

        // Input Process //
        ProcessInput(window);

        //glClearColor(background_color.x, background_color.y, background_color.z, background_color.w);
        glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



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

        if (!debugBox || !debugGrid) {

        mazeShader.setFloat("material.shininess",64.0f);
            cubeMesh.Draw(mazeShader, GL_TRIANGLES);
        }

        mazeShader.setFloat("material.shininess",32.0f);
        floorMesh.Draw(mazeShader, GL_TRIANGLES);
        mazeShader.setMat4("model", model);

        ceilingMesh.Draw(mazeShader,GL_TRIANGLES);
        // Floating light-source markers, rendered as small unlit cubes.
        lightShader.use();
        lightShader.setMat4("projection",projection);
        lightShader.setMat4("view",view);
        for (auto &light:lights) {
            glm::mat4 model  = glm::mat4(1.0);
            model = glm::translate(model,{light.position.x,light.position.y + 0.3, light.position.z});
            model = glm::scale(model,{0.1f,0.1f,0.1f});
            lightShader.setMat4("model",model);
            lightCube.Draw(lightShader,GL_TRIANGLES);


        }
        if (reachedExit) {
            particelShader.use();
            particelShader.setMat4("projection",projection);
            particelShader.setMat4("view",view);
            particelShader.setMat4("model",glm::mat4(1.0f));

            particles.Pour({exitPos.x,exitPos.y +0.5f,exitPos.z},12000,deltaTime);
            particles.Update(deltaTime);
            particles.Render();
        }


        // Debug overlays: wall bounding boxes / grid wireframe, drawn on
        // top with depth testing off so they're always visible.
        if (debugBox ) {
            glDisable(GL_DEPTH_TEST);
            glLineWidth(2.0f);

            Bbox.use();
            Bbox.setMat4("view", view);
            Bbox.setMat4("projection", projection);
            glm::mat4 model = glm::mat4(1.0f);
            Bbox.setMat4("model", model);
            bondingBoxMesh.Draw(Bbox, GL_LINES);
            glEnable(GL_DEPTH_TEST);

        }
        if (debugGrid) {
            glDisable(GL_DEPTH_TEST);
            glLineWidth(2.0f);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            Bbox.use();
            Bbox.setMat4("view", view);
            Bbox.setMat4("projection", projection);
            glm::mat4 model = glm::mat4(1.0f);
            Bbox.setMat4("model", model);
            gridMesh.Draw(Bbox,GL_LINES);
            wallMesh.Draw(Bbox,GL_TRIANGLES);
            glEnable(GL_DEPTH_TEST);
        }

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
    static bool fKeyWasPressed = false;
    bool fPressed = (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F));
    if (fPressed  && !fKeyWasPressed) {
        camera.fly = !camera.fly ;
        camera.collisionsOn = !camera.collisionsOn;
    }
    fKeyWasPressed = fPressed;
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
    static bool bKeyWasPressed = false;
    bool bPressed = (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_B));
    if (bPressed  && !bKeyWasPressed) {
        debugBox = !debugBox;
    }
    bKeyWasPressed= bPressed;
    static bool gKeyWasPressed = false;
    bool gPressed = (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_G));
    if (gPressed  && !gKeyWasPressed) {
        debugGrid = !debugGrid;
    }
    gKeyWasPressed = gPressed;

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

