#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "Core/Timer.h"
#include "Core/Camera.h"
#include "Scene/MainScene.h"
// #include "Scene/GameScene.h"

class SceneManager
{
protected:
    SDL_Renderer* m_renderer;                                   // 渲染器
    std::shared_ptr<TextureRes> m_textureRes;                   // 纹理资源
    std::shared_ptr<Camera> m_camera;                           // 相机
    std::shared_ptr<Timer> m_timer;                             // 游戏时钟

    std::shared_ptr<SceneObject> m_currentScene;

    std::vector<std::shared_ptr<SceneObject>> m_sceneTemplate;

public:
    SceneManager(
        SDL_Renderer* renderer,
        std::shared_ptr<TextureRes> res,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<Timer> timer
    );

    int handleEvent(SDL_Event& event);

    int updateScene();

    int renderScene();

    ~SceneManager();
};

#endif