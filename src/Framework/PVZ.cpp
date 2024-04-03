#include "Scene/GameScene.h"
#include <thread>
#include "SDL_image.h"
#include "Core/Timer.h"

#define FLUSH_DELAY 1000 / 45

// 全局变量 适配分辨率
SDL_Window* pvz_window = nullptr;
SDL_Renderer* pvz_renderer = nullptr;
unsigned int pvz_window_width = 800;
unsigned int pvz_window_height = 600;
bool QuitFlag = false;
// float pvz_window_scale = 1.0f;
Timer pvz_timer;
// int pvz_card_width = 53;
// int pvz_card_height = 71;

void RenderThread()
{
    SDL_Rect rect{ 0, 0, 100, 100 };
    while (!QuitFlag)
    {
        pvz_timer.updateTime();
        // 更新物体状态
        // ...

        // 清空屏幕
        SDL_SetRenderDrawColor(pvz_renderer, 0, 10, 100, 255);
        SDL_RenderClear(pvz_renderer);

        // 渲染图形
        SDL_SetRenderDrawColor(pvz_renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(pvz_renderer, 0, 100, 200, 100);
        SDL_RenderDrawLine(pvz_renderer, 100, 0, 100, 200);
        rect.x += 1;
        if (rect.x > pvz_window_width) rect.x = 0;
        SDL_RenderDrawRect(pvz_renderer, &rect);

        // 刷新屏幕
        SDL_RenderPresent(pvz_renderer);
        // 帧率控制
        if (pvz_timer.getDeltaTime() < FLUSH_DELAY)
        {
            SDL_Delay(FLUSH_DELAY - pvz_timer.getDeltaTime());
        }
    }

    SDL_Log("render thread quit!\n");
}

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (!SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1")) { SDL_Log("error: fail to set SDL_HINT_IME_SHOW_UI"); }
    if (!SDL_SetHint(SDL_HINT_IME_SUPPORT_EXTENDED_TEXT, "1")) { SDL_Log("error: fail to set SDL_HINT_IME_SUPPORT_EXTENDED_TEXT"); }

    pvz_window = SDL_CreateWindow("PVZ",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        pvz_window_width, pvz_window_height,
        SDL_WINDOW_SHOWN);
    if (pvz_window == nullptr) { SDL_Log("fail to create a window"); return 2; }

    pvz_renderer = SDL_CreateRenderer(pvz_window, -1, SDL_RENDERER_ACCELERATED);
    if (pvz_renderer == nullptr) { SDL_Log("fail to create a renderer"); return 2; }

    SDL_SetRenderDrawBlendMode(pvz_renderer, SDL_BLENDMODE_BLEND);

    std::thread render_thread(RenderThread);

    SDL_Event event;
    while (!QuitFlag)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                QuitFlag = true;
            }

        }
        
    }

    render_thread.join();

    SDL_DestroyRenderer(pvz_renderer);
    SDL_DestroyWindow(pvz_window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}