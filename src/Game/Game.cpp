#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <glm/glm.hpp>

#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"

Game::Game()
{
    isRunnung = false;
    isDebug = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
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

    // Setup window
    SDL_DisplayID primary_display = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(primary_display);
    windowWidth = mode->w;
    windowHeight = mode->h;
    Logger::Log("Window width - " + std::to_string(mode->w));
    Logger::Log("Window height - " + std::to_string(mode->h));
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
            if (sdlEvent.key.key == SDLK_D) isDebug = !isDebug;
        default:
            break;
        }
    }
}

void Game::LoadLevel(int level)
{
    // Add the Systems that need to be proccessed in game
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();


    // Adding assets to the asset store
    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
    assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
    assetStore->AddTexture(renderer, "tilemap-jungle", "./assets/tilemaps/jungle.png");

    // Load the tilemap
    int tileSize = 32;
    double tileScale = 2.f;
    int mapNumCols = 25;
    int mapNumRows = 20;

    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");

    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponent>
            (
                glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)),
                glm::vec2(tileScale, tileScale),
                0.0
            );
            tile.AddComponent<SpriteComponent>
            (
                "tilemap-jungle",
                tileSize,
                tileSize,
                srcRectX,
                srcRectY,
                0
            );
        }
    }
    mapFile.close();

    // Create an entity
    // Radar
    Entity radar = registry->CreateEntity();
    radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 200, 15.0), glm::vec2(3.0, 3.0), 0.0);
    radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 0.0, 0.0, 10);
    radar.AddComponent<AnimationComponent>(8, 5, true);
    // Choper
    Entity chopper = registry->CreateEntity();
    chopper.AddComponent<TransformComponent>(glm::vec2(200.0, 200.0), glm::vec2(2.0, 2.0), 0.0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(100.0, 0.0));
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 0.0, 0.0, 9);
    chopper.AddComponent<AnimationComponent>(2, 20, true);
    chopper.AddComponent<BoxColliderComponent>(32, 32);
    // Tank
    Entity tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(200.0, 400.0), glm::vec2(2.0, 2.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 0.0, 0.0, 2);
    tank.AddComponent<BoxColliderComponent>(32, 32);
    // Truck
    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(600.0, 400.0), glm::vec2(2.0, 2.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(-20.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 0.f, 0.f, 1);
    truck.AddComponent<BoxColliderComponent>(32, 32);
}

void Game::Setup()
{
    LoadLevel(1);
}

void Game::Update()
{
    // uint32_t timeout = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecPreviosFrame);
    // if (timeout > 0 && timeout <= MILLISECS_PER_FRAME) SDL_Delay(timeout);

    double deltaTime = (SDL_GetTicks() - millisecPreviosFrame) / 1000.f;

    millisecPreviosFrame = SDL_GetTicks();

    // Ask all systems to update
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update();

    // Update the registry to process the entities that are waiting to be created/deleted
    registry->Update();
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 29, 29, 29, 255);
    SDL_RenderClear(renderer);

    //TODO: Render all game objects...

    // Invoke all the systems that need to render
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
    if (isDebug) registry->GetSystem<RenderColliderSystem>().Update(renderer);

    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}