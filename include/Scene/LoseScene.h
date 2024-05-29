#ifndef LOSESCENE_H
#define LOSESCENE_H

#include "SceneObject.h"
#include "Core/AnimPlayer.h"

class LoseScene : public SceneObject {
protected:
    AnimPlayer m_animPlayer;
    uint64_t m_stopTimeCountDown;
public:
    LoseScene();

    // 返回场景的类型枚举值
    virtual SceneType getType() override;

    // 进入场景需要做的场景初始化操作
    virtual int enterScene() override;

    // 场景处理用户输入的事件
    // 返回跳转的场景
    virtual SceneType handleEvent(SDL_Event& event) override;

    // 更新场景以及场景内的物体
    // 返回跳转的场景
    virtual SceneType updateScene() override;

    // 退出场景执行的操作
    virtual int exitScene() override;

    // 渲染场景
    virtual int renderScene() override;

    ~LoseScene();
};

#endif