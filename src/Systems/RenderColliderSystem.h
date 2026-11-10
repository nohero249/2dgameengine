# pragma once

#include <SDL3/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

class RenderColliderSystem: public System {
    public:
        RenderColliderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void Update (SDL_Renderer* renderer, SDL_FRect& camera) {
            for (auto entity: GetSystemEntities()) {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto collider = entity.GetComponent<BoxColliderComponent>();


                SDL_FRect colliderRect = {
                    (transform.position.x + collider.offset.x - camera.x),
                    (transform.position.y + collider.offset.y - camera.y),
                    (collider.width * transform.scale.x),
                    (collider.height * transform.scale.y)
                };
                if (collider.isColliding) SDL_SetRenderDrawColor(renderer, 255, 0, 0 , 255);
                else SDL_SetRenderDrawColor(renderer, 255, 255, 0 , 255);
                SDL_RenderRect(renderer, &colliderRect);
            }
        }
};