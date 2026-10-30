#pragma once
#include <string>
#include <SDL3/SDL.h>

struct SpriteComponent 
{
    std::string assetId;
    int width;
    int height;
    int zIndex;
    SDL_FRect srcRect;

    SpriteComponent (
        std::string assetId = "",
        int width = 0,
        int height = 0,
        float srcRectX = 0.f,
        float srcRectY = 0.f,
        int zIndex = 0
    ) 
    {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->srcRect = {srcRectX, srcRectY, static_cast<float>(width), static_cast<float>(height)};
    }
};
