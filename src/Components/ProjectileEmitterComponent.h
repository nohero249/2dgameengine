#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

struct ProjectileEmitterComponent {
    glm::vec2 projectileVelocity;
    int repeatFreaquency;
    int projectileDuration;
    int hitPercentDamage;
    bool isFriendy;
    int lastEmissionTime;

    ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0), int repeatFreaquency = 0, int projectileDuration = 10000, int hitPercentDamage = 10, bool isFriendly = false) {
        this->projectileVelocity = projectileVelocity;
        this->repeatFreaquency = repeatFreaquency;
        this->projectileDuration = projectileDuration;
        this->hitPercentDamage = hitPercentDamage;
        this->lastEmissionTime = SDL_GetTicks();
    }
};