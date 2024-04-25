#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include <memory>

#include "SDL.h"
#include "Core/Timer.h"
#include "Core/Camera.h"
#include "Core/TextureRes.h"
#include "Core/AnimLoader.h"
#include "Core/AnimPlayer.h"

enum SceneType {
    Scene_MainScene,                // 游戏主界面
    Scene_SelectCardScene,          // 开始游戏后的选卡界面
    Scene_Select2GameScene,         // 选卡界面到游戏界面的过渡界面
    Scene_GameScene,                // 游戏过程中的界面
    Scene_MaxSceneIdx               // 最大的界面数量(代表无效界面)
};

class SceneObject {
protected:
    SDL_Renderer* m_renderer;
    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<TextureRes> m_textureRes;
public:
    SceneObject(
        SDL_Renderer* renderer,
        std::shared_ptr<Timer> timer,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<TextureRes> res
    ) :
        m_renderer(renderer),
        m_timer(timer),
        m_camera(camera),
        m_textureRes(res)
    {};

    // 返回场景的类型枚举值
    virtual SceneType getType() = 0;

    // 进入场景需要做的场景初始化操作
    virtual int enterScene() = 0;

    // 场景处理用户输入的事件
    // 返回跳转的场景
    virtual SceneType handleEvent(SDL_Event& event) = 0;

    // 更新场景以及场景内的物体
    // 返回跳转的场景
    virtual SceneType updateScene() = 0;

    // 退出场景执行的操作
    virtual int exitScene() = 0;

    // 渲染场景
    virtual int renderScene() = 0;

    virtual ~SceneObject() {};
};

#endif