#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <glm/glm.hpp>

#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"

Game::Game()
{
    isRunnung = false;
    Logger::Log("Game constructor called!");
}

Game::~Game()
{
    Logger::Log("Game destructor called!");
}

void Game::Initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        Logger::Err("Error initializing SDL3!");
        return;
    }

    // const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
    // if (displayMode)
    // {
    //     windowWidth = displayMode->w;
    //     windowHeight = displayMode->h;
    // }

    windowWidth = 800;
    windowHeight = 600;
    window = SDL_CreateWindow(
        NULL, 
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );
    if (!window)
    { 
        Logger::Err("Error creating SDL window!");
        return;
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        Logger::Err("Error creating SDL renderer!");
        return;        
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunnung = true;
}

void Game::Run()
{
    Logger::Log("Game run function start!");
    Setup();
    while (isRunnung)
    {
        ProcessInput();
        Update();
        Render();
    }
    Logger::Log("Game run function stop!");
}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
        case SDL_EVENT_QUIT:
            isRunnung = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (sdlEvent.key.key == SDLK_ESCAPE)
            {
                isRunnung = false;
                break;
            }
        default:
            break;
        }
    }
}

void Game::Setup()
{
    // TODO:
    // Entity tank = registry.CreateEntity();
    // tank.AddComponent<TransformComponent>();
    // tank.AddComponent ...
}

void Game::Update()
{
    // uint32_t timeout = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecPreviosFrame);
    // if (timeout > 0 && timeout <= MILLISECS_PER_FRAME) SDL_Delay(timeout);

    double deltaTime = (SDL_GetTicks() - millisecPreviosFrame) / 1000.f;

    millisecPreviosFrame = SDL_GetTicks();

    // TODO:
    // MovementSystem.Update();
    // ...
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 29, 29, 29, 255);
    SDL_RenderClear(renderer);

    //TODO: Render all game objects...

    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}