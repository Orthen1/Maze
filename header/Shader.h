//
// Created by teo on 25. 7. 2026.
//

#ifndef MAZE_SHADER_H
#define MAZE_SHADER_H
#pragma once
#include <string>
#include  <glad/glad.h>

#include <sstream>
#include <fstream>
#include <glm/glm.hpp>



class Shader {

public:
    unsigned int ID;
    Shader(const char *vertexPath, const char *fragmentPath);
   virtual ~Shader() = default;
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;


};



#endif //MAZE_SHADER_H
