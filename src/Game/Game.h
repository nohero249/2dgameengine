#pragma once

#include <SDL3/SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"

const int FPS = 5;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunnung;
        bool isDebug;
        uint64_t millisecPreviosFrame;
        SDL_Window* window{nullptr};
        SDL_Renderer* renderer{nullptr};

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore;

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

        int windowWidth{800};
        int windowHeight{600};
};