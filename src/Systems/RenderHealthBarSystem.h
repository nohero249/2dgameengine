#pragma once

#include <SDL3/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"

#include "../AssetStore/AssetStore.h"

class RenderHealthBarSystem: public System {
    public:
        RenderHealthBarSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<HealthComponent>();
        }

        void Update(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore, const SDL_FRect& camera) {
            for (auto entity: GetSystemEntities()) {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto& health = entity.GetComponent<HealthComponent>();
                const auto sprite = entity.GetComponent<SpriteComponent>();

                SDL_Color healthBarColor = {255, 255, 255, 255};
                if (health.healthPercentage >= 80) {
                    healthBarColor = {0, 255, 0, 255};
                }
                else if (health.healthPercentage < 80 && health.healthPercentage >= 40) {
                    healthBarColor = {255, 255, 0, 255};
                }
                else if (health.healthPercentage < 40 && health.healthPercentage >= 0) {
                    healthBarColor = {255, 0, 0, 255};
                }

                float healthBarWidth = 15.f;
                float healthBarHeight = 3.f;
                float healthBarPosX = transform.position.x + (sprite.width * transform.scale.x) - camera.x;
                float healthBarPosY = transform.position.y - camera.y;

                SDL_FRect healthBarRect = {
                    healthBarPosX,
                    healthBarPosY,
                    healthBarWidth * (health.healthPercentage / 100.f),
                    healthBarHeight
                };
                SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, healthBarColor.a);
                SDL_RenderFillRect(renderer, &healthBarRect);

                std::string healthText = std::to_string(health.healthPercentage);

                SDL_Surface* surface = TTF_RenderText_Blended(
                    assetStore->GetFont("pico8-font-5"), 
                    healthText.c_str(), 
                    0, 
                    healthBarColor
                );
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_DestroySurface(surface);
                
                float labelWidth = 0.f;
                float labelHeight = 0.f;
                SDL_GetTextureSize(texture, &labelWidth, &labelHeight);

                SDL_FRect healthBarTextRect = {
                    healthBarPosX,
                    healthBarPosY + 5,
                    labelWidth,
                    labelHeight
                };
                SDL_RenderTexture(renderer, texture, NULL, &healthBarTextRect);
                SDL_DestroyTexture(texture);
            }
        }
};