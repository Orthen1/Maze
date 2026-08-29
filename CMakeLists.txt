cmake_minimum_required(VERSION 4.2)
project(Maze)

set(CMAKE_CXX_STANDARD 20 )
include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1   # pin a stable release tag
)
FetchContent_MakeAvailable(glm)

# --- GLFW ---
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

if(WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
else ()
    add_compile_definitions(PLATFORM_LINUX)
endif ()




set(INCLUDE_DIRECTORIES ./)
include_directories(header src External)

#find_package(glfw3 REQUIRED)
#find_package(glm REQUIRED)
#find_package(OpenGL REQUIRED)

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
        External/glad/glad.h
        src/Light.cpp
        header/Batch.h
        src/Batch.cpp
)

add_executable(Maze ${SOURCE_FILES})

target_link_libraries(Maze PRIVATE glfw glm::glm)

if (WIN32)
    target_link_libraries(Maze PRIVATE opengl32)
else()
    target_link_libraries(Maze PRIVATE GL)
endif()
