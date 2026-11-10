#pragma once

#include <SDL3/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ProjectileComponent.h"

#include "../Logger/Logger.h"

class ProjectileEmitSystem: public System {
    public:
        ProjectileEmitSystem() {
            RequireComponent<ProjectileEmitterComponent>();
            RequireComponent<TransformComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressed);
        }

        void OnKeyPressed(KeyPressedEvent& event) {
            if (event.symbol == SDLK_SPACE) {
                Logger::Log("Space Pressed");
                for (auto entity: GetSystemEntities()) {
                    if (entity.HasComponent<CameraFollowComponent>()) {
                        const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
                        const auto transform = entity.GetComponent<TransformComponent>();
                        const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

                        // If parent entity has sprite, start the projectile position in the middle sprite
                        glm::vec2 projectilePostion = transform.position;
                        if (entity.HasComponent<SpriteComponent>()) {
                            auto sprite = entity.GetComponent<SpriteComponent>();
                            projectilePostion.x += (transform.scale.x * sprite.width / 2);
                            projectilePostion.y += (transform.scale.y * sprite.height / 2);
                        }

                        // If parent entity direction is controlled by the kyeboard keys, modify the direction
                        glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
                        int directionX = 0;
                        int directionY = 0;
                        if (rigidbody.velocity.x > 0) directionX = +1;
                        if (rigidbody.velocity.x < 0) directionX = -1;
                        if (rigidbody.velocity.y > 0) directionY = +1;
                        if (rigidbody.velocity.y < 0) directionY = -1;
                        projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
                        projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

                        // Create new pojectile entity and add it to the world
                        Entity projectile = entity.registry->CreateEntity();
                        projectile.Group("projectiles");
                        projectile.AddComponent<TransformComponent>(projectilePostion, glm::vec2(1.0, 1.0), 0.0);
                        projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
                        projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                        projectile.AddComponent<BoxColliderComponent>(4, 4);
                        projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendy, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
                    }
                }
            }
        }

        void Update(std::unique_ptr<Registry>& registry) {
            for (auto entity: GetSystemEntities()) {
                auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
                const auto transform = entity.GetComponent<TransformComponent>();

                // If emission frequency is zero, bypass re-emission logic
                if (projectileEmitter.repeatFreaquency == 0) continue;
                
                // Check if its time to re-emit a new projectile
                if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFreaquency) {
                    glm::vec2 projectilePosition = transform.position;
                    if (entity.HasComponent<SpriteComponent>()) {
                        const auto sprite = entity.GetComponent<SpriteComponent>();
                        projectilePosition.x += (transform.scale.x * sprite.width / 2);
                        projectilePosition.y += (transform.scale.y * sprite.height / 2);
                    }

                    Entity projectile = registry->CreateEntity();
                    projectile.Group("projectiles");
                    projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                    projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
                    projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                    projectile.AddComponent<BoxColliderComponent>(4, 4);
                    projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendy, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
                    // Update the projectile last emission time to the current time
                    projectileEmitter.lastEmissionTime = SDL_GetTicks();
                }
            }
        }
};