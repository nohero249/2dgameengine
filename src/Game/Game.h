#pragma once

#include <SDL3/SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Events/CollisionEvent.h"
#include "../EventBus/EventBus.h"

const int FPS = 5;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunnung;
        bool isDebug;
        uint64_t millisecPreviosFrame;
        SDL_Window* window{nullptr};
        SDL_Renderer* renderer{nullptr};
        SDL_FRect camera;

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore;
        std::unique_ptr<EventBus> eventBus;

    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
        void Setup();
        void LoadLevel(int level);

        static int windowWidth;
        static int windowHeight;
        static int mapWidth;
        static int mapHeight;
};