//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_LIGHT_H
#define MAZE_LIGHT_H
struct Light {
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 color;
};
#endif  // MAZE_LIGHT_H
