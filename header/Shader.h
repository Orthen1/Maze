//
// Created by teo on 25. 7. 2026.
//

#ifndef MAZE_SHADER_H
#define MAZE_SHADER_H
#pragma once
#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <sstream>
#include <string>

/**
 *  @brief Wraps an OpenGL shader program.
 *
 * Handles compiling and linking a vertex/fragment shader pair from file,
 * and provides helpers to activate the program and upload uniform values.
 */
class Shader {
   public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);

    virtual ~Shader() = default;
    // Actovates Shader for later calls;
    void use();

    // Sets uniforms in shaders
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;
    void setVec3(const std::string& name, glm::vec3 value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
};

#endif  // MAZE_SHADER_H
