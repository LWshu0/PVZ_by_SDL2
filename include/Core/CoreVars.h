#ifndef COREVARS_H
#define COREVARS_H

#include "SDL.h"
#include "SDL_image.h"
#include "ExtSDL.h"
#include "Timer.h"
#include "Camera.h"

class CoreVars
{
public:

    SDL_Renderer* renderer;
    SDL_Window* window;
    unsigned int window_width;
    unsigned int window_height;

    Timer timer;
    Camera camera;

    static CoreVars& getInstance();

    // 禁止复制和赋值
    CoreVars(CoreVars const&) = delete;
    void operator=(CoreVars const&) = delete;
private:
    CoreVars();
    ~CoreVars();
};


#endif