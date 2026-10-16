#pragma once

#include <SDL3/SDL.h>

const int FPS = 5;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunnung;
        uint64_t millisecPreviosFrame;
        SDL_Window* window{nullptr};
        SDL_Renderer* renderer{nullptr};

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

        int windowWidth{800};
        int windowHeight{600};
};