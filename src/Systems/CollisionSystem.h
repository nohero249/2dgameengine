#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem: public System {
    public:
        CollisionSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void Update() {
            auto entities = GetSystemEntities();

            if (entities.size() < 2) return; 

            // Loop all the entities that the system is interested in
            for (auto i = entities.begin(); i != entities.end(); i++) {
                Entity a = *i;

                const auto& aTransform = a.GetComponent<TransformComponent>();
                auto& aCollider = a.GetComponent<BoxColliderComponent>();

                for (auto j = std::next(i); j != entities.end(); j++) {
                    Entity b = *j;

                    const auto& bTransform = b.GetComponent<TransformComponent>();
                    auto& bCollider = b.GetComponent<BoxColliderComponent>();

                    // Perform the AABB collision check between entities a and b
                    bool collisionHappened = CheckAABBCollision(
                        aTransform.position.x + aCollider.offset.x,
                        aTransform.position.y + aCollider.offset.y,
                        aCollider.width,
                        aCollider.height,
                        bTransform.position.x + bCollider.offset.x,
                        bTransform.position.y + bCollider.offset.y,
                        bCollider.width,
                        bCollider.height
                    );

                    if (collisionHappened) {
                        aCollider.isColliding = true;
                        bCollider.isColliding = true;
                        Logger::Log("Entity " + std::to_string(a.GetId()) + " is colliding with entity " + std::to_string(b.GetId()));

                        // TODO: emit an event...
                    }
                    else {
                        aCollider.isColliding = false;
                        bCollider.isColliding = false;
                    }
                }
            }
        }

        bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
            return (
                aX < bX + bW &&
                aX + aW > bX &&
                aY < bY + bH &&
                aY + aH > bY
            );
        }
};