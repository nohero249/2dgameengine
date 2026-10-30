#pragma once

#include <glm/glm.hpp>

struct RigidBodyComponent
{
    glm::vec2 velocity;

    RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.f, 0.f))
    {
        this->velocity = velocity;
    }
};