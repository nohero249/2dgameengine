#pragma once

#include <SDL3/SDL.h>

struct AnimationComponent
{
    int numFrames;
    int currentFrame;
    int frameSpeedRate;
    bool isLoop;
    int startTime;

    AnimationComponent(int numFrames = 1, int framSpeedRate = 1, bool isLoop = true)
    {
        this->numFrames = numFrames;
        this->currentFrame = 1;
        this->frameSpeedRate = framSpeedRate;
        this->isLoop = isLoop;
        this->startTime = SDL_GetTicks();
    }
};
