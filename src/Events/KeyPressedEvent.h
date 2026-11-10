#pragma once

#pragma once

#include <SDL3/SDL.h>

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class KeyPressedEvent: public Event {
    public:
        SDL_Keycode symbol;
        KeyPressedEvent(SDL_Keycode symbol): symbol(symbol) {}
};