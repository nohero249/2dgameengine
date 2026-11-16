#include <SDL3_image/SDL_image.h>

#include <iostream>

#include "./AssetStore.h"
#include "../Logger/Logger.h"

AssetStore::AssetStore()
{
    Logger::Log("AssetStore constructor called");
}

AssetStore::~AssetStore()
{
    ClearAssets();
    Logger::Log("AssetStore destructor called");
}

void AssetStore::ClearAssets()
{
    for (auto texture: textures) SDL_DestroyTexture(texture.second);
    textures.clear();

    for (auto font: fonts) TTF_CloseFont(font.second);
    fonts.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath)
{
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    // Add the texture to the map
    textures.emplace(assetId, texture);
    Logger::Log("New texture added to the Asset Store with id = " + assetId);
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId)
{
    return textures[assetId];
}

void AssetStore::AddFont(const std::string& assetId, const std::string& filePath, int fontSize) {
    TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
    if (!font) {
        Logger::Err("Failed to load font: " + std::string(SDL_GetError()));
        return;        
    }
    fonts.emplace(assetId, font);
    Logger::Log("New font added to the Asset Store with id = " + assetId);
}

TTF_Font* AssetStore::GetFont(const std::string& assetId) {
    if (fonts[assetId]) return fonts[assetId];
    else {
        Logger::Err("Font on id " + assetId + " not found");
        return 0;
    }
}