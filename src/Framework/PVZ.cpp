#include <thread>
#include "Core/GlobalVars.h"
#include "Scene/GameScene.h"
#include "Manager/MapManager.h"
#include "Manager/ProductManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/TaskManager.h"
#include "Manager/CardManager.h"
#include "Manager/SceneManager.h"

#define FLUSH_DELAY 1000 / 45

bool QuitFlag = false;
// float pvz_window_scale = 1.0f;
// int pvz_card_width = 53;
// int pvz_card_height = 71;
std::shared_ptr<MapManager> map_manager;
std::shared_ptr<ProductManager> product_manager;
std::shared_ptr<PlantManager> plant_manager;
std::shared_ptr<ZombieManager> zombie_manager;
std::shared_ptr<TaskManager> task_manager;
std::shared_ptr<CardManager> card_manager;
std::shared_ptr<SceneManager> scene_manager;

SpinLock handle_update_render_spinlock;

void RenderThread()
{

    while (!QuitFlag)
    {
        // 更新时钟
        GlobalVars::getInstance().timer.updateTime();

        /*************************************************************
         *                            lock                           *
        **************************************************************/
        handle_update_render_spinlock.lock();
        // 更新场景
        scene_manager->updateScene();

        // 清空屏幕
        SDL_SetRenderDrawColor(GlobalVars::getInstance().renderer, 0, 10, 100, 255);
        SDL_RenderClear(GlobalVars::getInstance().renderer);

        // 渲染场景
        scene_manager->renderScene();
        /*************************************************************
         *                          unlock                           *
        **************************************************************/
        handle_update_render_spinlock.unlock();

        // 刷新屏幕
        SDL_RenderPresent(GlobalVars::getInstance().renderer);

        // 帧率控制
        if (GlobalVars::getInstance().timer.getDeltaTime() < FLUSH_DELAY)
        {
            SDL_Delay(FLUSH_DELAY - GlobalVars::getInstance().timer.getDeltaTime());
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

    // 创建管理者
    map_manager = std::make_shared<MapManager>();
    product_manager = std::make_shared<ProductManager>(100, 100);
    plant_manager = std::make_shared<PlantManager>();
    zombie_manager = std::make_shared<ZombieManager>();
    task_manager = std::make_shared<TaskManager>();
    card_manager = std::make_shared<CardManager>();
    scene_manager = std::make_shared<SceneManager>(
        map_manager, product_manager, plant_manager, zombie_manager, task_manager, card_manager
    );

    // 关联管理者
    plant_manager->initilizeManagers(map_manager, product_manager, zombie_manager);
    zombie_manager->initilizeManagers(map_manager, product_manager, plant_manager);
    task_manager->initilizeManagers(map_manager, zombie_manager);
    card_manager->initilizeManagers(map_manager, plant_manager, product_manager);
    product_manager->initilizeManagers(map_manager);

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

            scene_manager->handleEvent(event);

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    QuitFlag = true;
                    break;
                case SDLK_SPACE:
                    if (GlobalVars::getInstance().timer.isPause()) GlobalVars::getInstance().timer.start();
                    else GlobalVars::getInstance().timer.pause();
                    break;
                case SDLK_UP:
                    GlobalVars::getInstance().camera.move(0, -5.0f);
                    break;
                case SDLK_DOWN:
                    GlobalVars::getInstance().camera.move(0, 5.0f);
                    break;
                case SDLK_LEFT:
                    GlobalVars::getInstance().camera.move(-5.0f, 0);
                    break;
                case SDLK_RIGHT:
                    GlobalVars::getInstance().camera.move(5.0f, 0);
                    break;
                case SDLK_1:
                    zombie_manager->addZombie(ZombieType::ZombieNormal, 0, 7);
                    break;
                case SDLK_2:
                    zombie_manager->addZombie(ZombieType::ZombieNormal, 1, 7);
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

    // 取消关联(因存在循环引用, 需要释放内部的智能指针)
    plant_manager->releaseManagers();
    zombie_manager->releaseManagers();
    task_manager->releaseManagers();
    card_manager->releaseManagers();
    product_manager->releaseManagers();

    SDL_DestroyRenderer(GlobalVars::getInstance().renderer);
    SDL_DestroyWindow(GlobalVars::getInstance().window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}