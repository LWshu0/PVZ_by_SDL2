#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include <memory>

#include "SDL.h"

enum SceneType {
    Scene_MainScene,                // 游戏主界面
    Scene_SelectCardScene,          // 开始游戏后的选卡界面
    Scene_Select2GameScene,         // 选卡界面到游戏界面的过渡界面
    Scene_GameScene,                // 游戏过程中的界面
    Scene_WinScene,                 // 游戏获胜界面
    Scene_LoseScene,                // 游戏失败界面
    Scene_MaxSceneIdx               // 最大的界面数量(代表无效界面)
};

class SceneObject {
protected:

public:
    SceneObject() {};

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