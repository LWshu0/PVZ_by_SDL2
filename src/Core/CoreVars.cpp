#include "Core/CoreVars.h"

CoreVars& CoreVars::getInstance()
{
    static CoreVars instance;
    return instance;
}

CoreVars::CoreVars() :
    renderer(nullptr),
    window(nullptr),
    window_width(0),
    window_height(0)
{

    window_width = 800;
    window_height = 600;

    window = SDL_CreateWindow(
        "PVZ",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        // SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
        SDL_WINDOW_SHOWN
    );
    if (window == nullptr) { SDL_Log("fail to create a window"); return; }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) { SDL_Log("fail to create a renderer"); return; }
    SDL_RenderSetLogicalSize(renderer, 800, 600);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    camera.initilize(0, 0, 800, 600);
}

CoreVars::~CoreVars()
{}