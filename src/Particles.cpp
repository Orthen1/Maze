//
// Created by teo on 29. 8. 2026.
//

#include "../header/Particles.h"

void Particles::Update(float deltaTime) {
    ttl -= deltaTime;
    position += velocity * deltaTime;
}
