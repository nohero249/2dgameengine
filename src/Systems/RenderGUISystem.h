#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_sdlrenderer3.h>
#include <glm/glm.hpp>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components//BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"


class RenderGUISystem: public System {
    public:
        RenderGUISystem() = default;

        void Update(SDL_Renderer* renderer, std::unique_ptr<Registry>& registry, const SDL_FRect& camera, float deltaTime) {
            // Imgui render
            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize;
            if (ImGui::Begin("Spawn enemies", NULL, windowFlags)) {

                // TransformComponent values
                static int enemyPosX = 0;
                static int enemyPosY = 0;
                static int enemyScaleX = 1;
                static int enemyScaleY = 1;
                static float enemyRotation = 0.f;
                // RigidBodyComponent values
                static float enemyVelocityX = 0.f;
                static float enemyVelocityY = 0.f;
                // SpriteComponent values
                const char* sprites[] = {"tank-image", "truck-image"};
                static int selectedSpriteIndex = 0;
                // ProjectileEmitterComponent values
                static float enemyProjectileAngle = 0.f;
                static float enemyProjectileSpeed = 100.f;
                static int enemyProjectileRepeatFrequency = 10;
                static int enemyProjectileDuration = 10;
                static int enemyProjectileHitDamage = 10;
                // HealthComponent values
                static int enemyHealth = 100;

                if(ImGui::CollapsingHeader("Enemy TransfromComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("x position", &enemyPosX);
                    ImGui::InputInt("y position", &enemyPosY);
                    ImGui::SliderInt("x scale", &enemyScaleX, 1, 10);
                    ImGui::SliderInt("y scale", &enemyScaleY, 1, 10);
                    ImGui::SliderAngle("rotation", &enemyRotation, 0, 360);
                }
                ImGui::Spacing();
                if(ImGui::CollapsingHeader("Enemy RigidBodyComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputFloat("x velocity", &enemyVelocityX, 0.1f, 1.f, "%.1f");
                    ImGui::InputFloat("y velocity", &enemyVelocityY, 0.1f, 1.f, "%.1f");
                }
                ImGui::Spacing();
                if(ImGui::CollapsingHeader("Enemy SpriteComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Combo("texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
                }
                ImGui::Spacing();
                if(ImGui::CollapsingHeader("Enemy ProjectileEmitterComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SliderAngle("projectile angle", &enemyProjectileAngle, 0, 360);
                    ImGui::SliderFloat("projectile speed", &enemyProjectileSpeed, 10, 10000, "%.1f");
                    ImGui::InputInt("projectile repeat (sec)", &enemyProjectileRepeatFrequency);
                    ImGui::InputInt("projectile duration (sec)", &enemyProjectileDuration);
                    ImGui::InputInt("projectile hit damage", &enemyProjectileHitDamage);
                }
                ImGui::Spacing();
                if(ImGui::CollapsingHeader("Enemy HealthComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("HealthPoints", &enemyHealth);
                }
                ImGui::Spacing();
                if (ImGui::Button("Spawn")) {
                    Entity enemy = registry->CreateEntity();
                    enemy.Group("enemies");
                    enemy.AddComponent<TransformComponent>(glm::vec2(enemyPosX, enemyPosY), glm::vec2(enemyScaleX, enemyScaleY), glm::degrees(enemyRotation));
                    enemy.AddComponent<RigidBodyComponent>(glm::vec2(enemyVelocityX, enemyVelocityY));
                    enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32, 32, 1);
                    enemy.AddComponent<BoxColliderComponent>(25, 18, glm::vec2(5, 7));
                    double projectileVelocityX = cos(enemyProjectileAngle) * enemyProjectileSpeed;
                    double projectileVelocityY = sin(enemyProjectileAngle) * enemyProjectileSpeed;
                    enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projectileVelocityX, projectileVelocityY), enemyProjectileRepeatFrequency * 1000, enemyProjectileDuration * 1000, enemyProjectileHitDamage, false);
                    enemy.AddComponent<HealthComponent>(enemyHealth);

                }
            }
            ImGui::End();

            // Display a mouse x and y position
            windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(0.4f);

                        // 1. Накапливаем время и кадры (делаем это ОДИН раз за кадр, до цикла по сущностям)
            fpsTimer += deltaTime;
            frameCount++;

            // Обновляем отображаемый FPS каждые 0.5 секунды (можно поставить 0.25f для более частой реакции)
            if (fpsTimer >= 0.2f) {
                currentFps = static_cast<int>(frameCount / fpsTimer); // автоматическое отбрасывание дробной части
                fpsTimer = 0.0f;
                frameCount = 0;
            }

            if (ImGui::Begin("Debug", NULL, windowFlags)) {
                ImGui::Text(
                    "FPS: %d | Mouse coordinates (x=%.1f, y=%.1f)",
                    currentFps,
                    ImGui::GetIO().MousePos.x + camera.x,
                    ImGui::GetIO().MousePos.y + camera.y
                );
            }
            ImGui::End();
            
            ImGui::Render();
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        }
    private:
        float fpsTimer = 0.0f;
        int frameCount = 0;
        int currentFps = 0;
};