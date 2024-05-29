#include "Core/GlobalVars.h"
#include "Manager/CardManager.h"
#include "Manager/MapManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ProductManager.h"
#include "Manager/SceneManager.h"
#include "Manager/TaskManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/UsrInfoManager.h"

GlobalVars& GlobalVars::getInstance()
{
    static GlobalVars instance;
    return instance;
}

GlobalVars::GlobalVars()
{
    window_width = 800;
    window_height = 600;

    window = SDL_CreateWindow("PVZ",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) { SDL_Log("fail to create a window"); return; }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) { SDL_Log("fail to create a renderer"); return; }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    camera.initilize(0, 0, 800, 600);
    textureRes.initilize("resource/resource.xml", "reanim");

}