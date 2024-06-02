#include <thread>
#include "SDL.h"
#include "Resource/TextureRes.h"
#include "Resource/Dictionary.h"

#define FLUSH_DELAY 1000 / 45

// 全局变量 适配分辨率
SDL_Window* pvz_window = nullptr;
SDL_Renderer* pvz_renderer = nullptr;
unsigned int pvz_window_width = 800;
unsigned int pvz_window_height = 600;
bool QuitFlag = false;

std::shared_ptr<TextureRes> texture_res;
std::shared_ptr<Dictionary> dict;

void RenderThread()
{

    while (!QuitFlag)
    {
        // 更新时钟
        // pvz_timer->updateTime();
        // 更新物体状态
        // ...

        // 清空屏幕
        SDL_SetRenderDrawColor(pvz_renderer, 0, 10, 100, 255);
        SDL_RenderClear(pvz_renderer);

        // 渲染图形
        dict->render("0123456789", 0, 0, 2.0f);
        // 刷新屏幕
        SDL_RenderPresent(pvz_renderer);
        // 帧率控制
        // if (pvz_timer->getDeltaTime() < FLUSH_DELAY)
        // {
        //     SDL_Delay(FLUSH_DELAY - pvz_timer->getDeltaTime());
        // }
    }

    SDL_Log("render thread quit!\n");
}

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (!SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1")) { SDL_Log("error: fail to set SDL_HINT_IME_SHOW_UI"); }
    if (!SDL_SetHint(SDL_HINT_IME_SUPPORT_EXTENDED_TEXT, "1")) { SDL_Log("error: fail to set SDL_HINT_IME_SUPPORT_EXTENDED_TEXT"); }

    pvz_window = SDL_CreateWindow("DictionaryTest",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        pvz_window_width, pvz_window_height,
        SDL_WINDOW_SHOWN);
    if (pvz_window == nullptr) { SDL_Log("fail to create a window"); return 2; }

    pvz_renderer = SDL_CreateRenderer(pvz_window, -1, SDL_RENDERER_ACCELERATED);
    if (pvz_renderer == nullptr) { SDL_Log("fail to create a renderer"); return 2; }

    SDL_SetRenderDrawBlendMode(pvz_renderer, SDL_BLENDMODE_BLEND);

    texture_res = std::make_shared<TextureRes>(pvz_renderer, "resource/resource.xml", "reanim");
    dict = std::make_shared<Dictionary>(pvz_renderer, texture_res, "data/_BrianneTod16.png", "data/BrianneTod16.txt", SDL_Color{ 255,0,0,255 });

    std::thread render_thread(RenderThread);

    SDL_Event event;
    while (!QuitFlag)
    {
        if (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                QuitFlag = true;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    QuitFlag = true;
                    break;
                case SDLK_SPACE:

                    break;
                case SDLK_UP:

                    break;
                case SDLK_DOWN:

                    break;
                case SDLK_LEFT:

                    break;
                case SDLK_RIGHT:

                    break;
                default:
                    break;
                }
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