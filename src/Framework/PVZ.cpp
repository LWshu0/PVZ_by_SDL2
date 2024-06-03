#include <thread>
#include "Manager/CardManager.h"
#include "Manager/MapManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ProductManager.h"
#include "Manager/SceneManager.h"
#include "Manager/TaskManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/UsrInfoManager.h"

// 全局单例
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"

#define FLUSH_DELAY 1000 / 45

bool QuitFlag = false;

SpinLock handle_update_render_spinlock;

void RenderThread()
{

    while (!QuitFlag)
    {
        // 更新时钟
        CoreVars::getInstance().timer.updateTime();

        /*************************************************************
         *                            lock                           *
        **************************************************************/
        handle_update_render_spinlock.lock();
        // 更新场景
        Managers::getInstance().sceneManager->updateScene();

        // 清空屏幕
        SDL_SetRenderDrawColor(CoreVars::getInstance().renderer, 0, 10, 100, 255);
        SDL_RenderClear(CoreVars::getInstance().renderer);

        // 渲染场景
        Managers::getInstance().sceneManager->renderScene();
        /*************************************************************
         *                          unlock                           *
        **************************************************************/
        handle_update_render_spinlock.unlock();

        // 刷新屏幕
        SDL_RenderPresent(CoreVars::getInstance().renderer);

        // 帧率控制
        if (CoreVars::getInstance().timer.getDeltaTime() < FLUSH_DELAY)
        {
            SDL_Delay(FLUSH_DELAY - CoreVars::getInstance().timer.getDeltaTime());
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
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) { SDL_Log("error: fail to set SDL_HINT_RENDER_SCALE_QUALITY"); }
    // SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "1");

    // 全局变量的初始化
    CoreVars::getInstance();
    ResVars::getInstance();
    // usrinfo 必须首先初始化
    Managers::getInstance().usrinfoManager = std::make_shared<UsrInfoManager>();
    Managers::getInstance().usrinfoManager->read();
    
    Managers::getInstance().mapManager = std::make_shared<MapManager>();
    Managers::getInstance().productManager = std::make_shared<ProductManager>(100, 100);
    Managers::getInstance().plantManager = std::make_shared<PlantManager>();
    Managers::getInstance().zombieManager = std::make_shared<ZombieManager>();
    Managers::getInstance().taskManager = std::make_shared<TaskManager>();
    Managers::getInstance().cardManager = std::make_shared<CardManager>();
    Managers::getInstance().cardManager->initilizeManagers();
    Managers::getInstance().sceneManager = std::make_shared<SceneManager>();

    std::thread render_thread(RenderThread);

    SDL_Event event;
    while (!QuitFlag)
    {
        if (SDL_WaitEvent(&event))
        {
            /*************************************************************
            *                            lock                           *
            **************************************************************/
            handle_update_render_spinlock.lock();

            if (event.type == SDL_QUIT)
            {
                QuitFlag = true;
            }

            Managers::getInstance().sceneManager->handleEvent(event);

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    QuitFlag = true;
                    break;
                case SDLK_SPACE:
                    if (CoreVars::getInstance().timer.isPause()) CoreVars::getInstance().timer.start();
                    else CoreVars::getInstance().timer.pause();
                    break;
                case SDLK_UP:
                    CoreVars::getInstance().camera.move(0, -5.0f);
                    break;
                case SDLK_DOWN:
                    CoreVars::getInstance().camera.move(0, 5.0f);
                    break;
                case SDLK_LEFT:
                    CoreVars::getInstance().camera.move(-5.0f, 0);
                    break;
                case SDLK_RIGHT:
                    CoreVars::getInstance().camera.move(5.0f, 0);
                    break;
                case SDLK_1:
                    Managers::getInstance().zombieManager->addZombie(ZombieType::ZombieNormal, 0, 7);
                    break;
                case SDLK_2:
                    Managers::getInstance().zombieManager->addZombie(ZombieType::ZombieNormal, 1, 7);
                    break;
                default:
                    break;
                }
            }
            /*************************************************************
            *                          unlock                           *
            **************************************************************/
            handle_update_render_spinlock.unlock();
        }
    }

    render_thread.join();
    // 释放 SDL 资源
    ResVars::release();
    SDL_DestroyRenderer(CoreVars::getInstance().renderer);
    SDL_DestroyWindow(CoreVars::getInstance().window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}