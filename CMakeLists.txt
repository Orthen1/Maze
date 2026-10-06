cmake_minimum_required(VERSION 4.2)
project(Maze)

set(CMAKE_CXX_STANDARD 20 )
include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)




set(INCLUDE_DIRECTORIES ./)
include_directories(header src External)

#find_package(glfw3 REQUIRED)
#find_package(glm REQUIRED)
#find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

set(SOURCE_FILES src/main.cpp src/glad.c External/stb_image/stb_image.cpp
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
        src/Player.cpp
        header/Player.h
        header/Player.h
        src/Spell.cpp
        header/Spell.h
        header/BoundingShapes.h
        src/BoundingShapes.cpp
        src/BoundingShapes.cpp
        header/enums.h
)

add_executable(Maze ${SOURCE_FILES})

if(WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)

    target_link_options(Maze PRIVATE
            -static-libgcc
            -static-libstdc++
            -Wl,-Bstatic,-lwinpthread,-Bdynamic

    )

    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    foreach(DLL libwinpthread-1.dll libgcc_s_seh-1.dll libstdc++-6.dll)
        if(EXISTS "${MINGW_BIN_DIR}/${DLL}")
            add_custom_command(TARGET Maze POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MINGW_BIN_DIR}/${DLL}"
                    $<TARGET_FILE_DIR:Maze>
            )
        endif()
    endforeach()

    target_link_libraries(Maze PRIVATE opengl32)
else()
    add_compile_definitions(PLATFORM_LINUX)

    target_link_libraries(Maze PRIVATE GL)
endif()
target_link_libraries(Maze PRIVATE glfw glm::glm assimp::assimp)

