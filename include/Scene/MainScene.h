#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Core/UI/wsShape.h"
#include "Core/UI/wsTriangles.h"
#include "Scene/SceneObject.h"

class MainScene:public SceneObject
{
protected:
    // 定义按钮
    enum MainSceneButtonType {
        MainSceneButton_StartAdventure,
        MainSceneButton_MiniGame,
        MainSceneButton_Challenge,
        MainSceneButton_Survival,
        MainScene_MaxButtonNum
    };
    enum MainSceneState {
        EnterScene,     // 墓碑升起
        WoodDrop,       // 告示牌掉落
        Idle            // 空闲
    } m_state;

    std::vector<std::unique_ptr<wsShape>> m_buttonRange;

    // 主界面动画播放器
    AnimPlayer m_animPlayer;

    int m_hoverButtonIdx;                           // 当前 hover 的按钮编号
    int m_clickButtonIdx;                           // 当前 click 的按钮编号
    std::vector<SDL_FPoint> m_buttonHoverOffset;    // 按钮在 click 状态下的偏移量
    std::vector<int> m_buttonTrackIdx;              // 按钮的动画轨道编号
    std::vector<SDL_Texture*> m_buttonHoverTexture; // 按钮处于 hover 状态下的纹理
    std::vector<SceneType> m_jumpSceneType;         // 点击按钮跳转的页面
public:
    MainScene();

    virtual SceneType getType() override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual SceneType updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;

    virtual ~MainScene();

protected:
    // 修改动画状态
    int setAnimState(MainSceneState to_state);
    // 必需传入一个枚举
    inline bool isValidButton(int enum_type) { return enum_type != MainSceneButtonType::MainScene_MaxButtonNum; }
    // 按钮是否是 hover 状态
    inline bool isHoverButton(int enum_type) { return enum_type == m_hoverButtonIdx; }
};

#endif