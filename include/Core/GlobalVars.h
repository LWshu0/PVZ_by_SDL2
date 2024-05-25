#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include "SDL.h"
#include "SDL_image.h"
#include "ExtSDL.h"
#include "Timer.h"
#include "Camera.h"
#include "TextureRes.h"

class GlobalVars {
public:
    static GlobalVars& getInstance();

    // 禁止复制和赋值
    GlobalVars(GlobalVars const&) = delete;
    void operator=(GlobalVars const&) = delete;

    SDL_Renderer* renderer;
    SDL_Window* window;
    unsigned int window_width;
    unsigned int window_height;

    Timer timer;
    Camera camera;
    TextureRes textureRes;
private:
    GlobalVars();
};

#endif