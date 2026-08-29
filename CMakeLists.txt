cmake_minimum_required(VERSION 4.2)
project(Maze)

set(CMAKE_CXX_STANDARD 20 )

set(INCLUDE_DIRECTORIES ./)
include_directories(header src)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(SOURCE_FILES src/main.cpp src/glad.c src/stb_image.cpp
        src/Shader.cpp
        header/Shader.h
        src/Camera.cpp
        header/Camera.h
        src/Mesh.cpp
        header/Mesh.h
        src/Grid.cpp
        header/Grid.h
        header/Grid.h
        header/main.h
        src/Particles.cpp
        header/Particles.h
        header/Particles.h
        header/Light.h
        src/ParticlesPool.cpp
        header/ParticlesPool.h
        header/BoundingBox.h
)

add_executable(Maze ${SOURCE_FILES})

target_link_libraries(Maze PRIVATE glfw)



