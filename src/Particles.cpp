//
// Created by teo on 29. 8. 2026.
//

#include "../header/Particles.h"

void Particles::Update(float deltaTime) {
    if (ttl <= 0) {
        alive = false;
    }
    ttl -= deltaTime;
    position += velocity * deltaTime;
}
