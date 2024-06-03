#include "Scene/MainScene.h"
// 全局单例
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"
/*
fps: 20
0: anim_open -> range: [0, 705] total: [0, 705]
1: anim_sign -> range: [13, 41] total: [0, 705]
2: anim_idle -> range: [41, 78] total: [0, 705]
3: anim_grass -> range: [78, 103] total: [0, 705]
4: anim_flower2 -> range: [103, 161] total: [0, 705]
5: anim_flower1 -> range: [161, 179] total: [0, 705]
6: anim_flower3 -> range: [179, 198] total: [0, 705]
7: anim_start -> range: [0, 705] total: [0, 705]
8: anim_cloud1 -> range: [198, 336] total: [0, 705]
9: anim_cloud7 -> range: [336, 422] total: [0, 705]
10: anim_cloud2 -> range: [422, 503] total: [0, 705]
11: anim_cloud4 -> range: [503, 705] total: [0, 705]
12: anim_cloud6 -> range: [569, 639] total: [0, 705]
13: anim_cloud5 -> range: [639, 705] total: [0, 705]
14: SelectorScreen_BG -> range: [0, 705] total: [0, 705]
15: Cloud1 -> range: [198, 336] total: [0, 705]
16: Cloud7 -> range: [336, 422] total: [0, 705]
17: Cloud2 -> range: [422, 503] total: [0, 705]
18: Cloud4 -> range: [503, 569] total: [0, 705]
19: Cloud6 -> range: [569, 639] total: [0, 705]
20: Cloud5 -> range: [639, 705] total: [0, 705]
21: SelectorScreen_BG_Center -> range: [0, 705] total: [0, 705]
22: SelectorScreen_BG_Left -> range: [0, 705] total: [0, 705]
23: SelectorScreen_BG_Right -> range: [0, 705] total: [0, 705]
24: almanac_key_shadow -> range: [0, 705] total: [0, 705]
25: SelectorScreen_Survival_shadow -> range: [0, 705] total: [0, 705]
26: SelectorScreen_Survival_button -> range: [0, 705] total: [0, 705]
27: SelectorScreen_Challenges_shadow -> range: [0, 705] total: [0, 705]
28: SelectorScreen_Challenges_button -> range: [0, 705] total: [0, 705]
29: SelectorScreen_ZenGarden_shadow -> range: [0, 705] total: [0, 705]
30: SelectorScreen_ZenGarden_button -> range: [0, 705] total: [0, 705]
31: SelectorScreen_StartAdventure_shadow -> range: [0, 705] total: [0, 705]
32: SelectorScreen_StartAdventure_button -> range: [0, 705] total: [0, 705]
33: SelectorScreen_Adventure_shadow -> range: [0, 705] total: [0, 705]
34: SelectorScreen_Adventure_button -> range: [0, 705] total: [0, 705]
35: leaf3 -> range: [78, 103] total: [0, 705]
36: leaf2 -> range: [78, 103] total: [0, 705]
37: leaf22 -> range: [78, 103] total: [0, 705]
38: leaf_SelectorScreen_Leaves -> range: [78, 103] total: [0, 705]
39: leaf4 -> range: [78, 103] total: [0, 705]
40: leaf5 -> range: [78, 103] total: [0, 705]
41: leaf1 -> range: [78, 103] total: [0, 705]
42: flower1 -> range: [179, 198] total: [0, 705]
43: flower2 -> range: [103, 161] total: [0, 705]
44: flower3 -> range: [161, 179] total: [0, 705]
45: woodsign1 -> range: [13, 78] total: [0, 705]
46: woodsign2 -> range: [13, 78] total: [0, 705]
47: woodsign3 -> range: [13, 78] total: [0, 705]
*/
MainScene::MainScene() :
    SceneObject(),
    m_animPlayer(
        std::make_shared<AnimLoader>("reanim/SelectorScreen.reanim"),
        SDL_FPoint{ 0.0f, 0.0f }
    ),
    m_state(MainSceneState::Invalid),
    m_hoverButtonIdx(MainSceneButtonType::MainScene_MaxButtonNum)
{
    m_buttonRange.resize(MainSceneButtonType::MainScene_MaxButtonNum);
    m_buttonHoverOffset.resize(MainSceneButtonType::MainScene_MaxButtonNum);
    m_buttonTrackIdx.resize(MainSceneButtonType::MainScene_MaxButtonNum);
    m_buttonHoverTexture.resize(MainSceneButtonType::MainScene_MaxButtonNum);
    m_jumpSceneType.resize(MainSceneButtonType::MainScene_MaxButtonNum);
    m_hoverButtonIdx = MainSceneButtonType::MainScene_MaxButtonNum;
    m_clickButtonIdx = MainSceneButtonType::MainScene_MaxButtonNum;
    {
        SDL_FPoint base_point{ 405.0f, 65.0f };
        std::vector<SDL_FPoint> points{
            // 按钮主体
            SDL_FPoint{ (base_point.x + 9.0f), (base_point.y + 37.0f) },
            SDL_FPoint{ (base_point.x + 5.0f), (base_point.y + 93.0f) },
            SDL_FPoint{ (base_point.x + 316.0f), (base_point.y + 61.0f) },
            SDL_FPoint{ (base_point.x + 307.0f), (base_point.y + 134.0f) },
            // 上方小突起
            SDL_FPoint{ (base_point.x + 93.0f), (base_point.y + 8.0f) },
            SDL_FPoint{ (base_point.x + 62.0f), (base_point.y + 45.0f) },
            SDL_FPoint{ (base_point.x + 209.0f), (base_point.y + 6.0f) },
            SDL_FPoint{ (base_point.x + 240.0f), (base_point.y + 57.0f) }
        };
        std::vector<int> idx{ 0,1,2,1,3,2 ,4,5,6,5,7,6 };
        m_buttonRange[MainSceneButtonType::MainSceneButton_StartAdventure] = std::make_unique<wsTriangles>(points, idx);
        m_buttonHoverOffset[MainSceneButtonType::MainSceneButton_StartAdventure] = SDL_FPoint{ 3.0f, 3.0f };
        m_buttonTrackIdx[MainSceneButtonType::MainSceneButton_StartAdventure] = 34;
        m_buttonHoverTexture[MainSceneButtonType::MainSceneButton_StartAdventure] = ResVars::getInstance().textureRes.getTextureFrom("reanim/SelectorScreen_StartAdventure_Highlight.png");
        m_jumpSceneType[MainSceneButtonType::MainSceneButton_StartAdventure] = SceneType::Scene_SelectCardScene;
    }
    /****************************************************************************/
    {
        SDL_FPoint base_point{ 406.0f, 173.1f };
        std::vector<SDL_FPoint> points{
            SDL_FPoint{  (base_point.x + 7.0f),  (base_point.y + 7.0f) },
            SDL_FPoint{  (base_point.x + 8.0f),  (base_point.y + 73.0f) },
            SDL_FPoint{  (base_point.x + 307.0f),  (base_point.y + 55.0f) },
            SDL_FPoint{  (base_point.x + 292.0f),  (base_point.y + 127.0f) }
        };
        std::vector<int> idx{ 0,1,2,1,3,2 };
        m_buttonRange[MainSceneButtonType::MainSceneButton_MiniGame] = std::make_unique<wsTriangles>(points, idx);
        m_buttonHoverOffset[MainSceneButtonType::MainSceneButton_MiniGame] = SDL_FPoint{ 3.0f, 3.0f };
        m_buttonTrackIdx[MainSceneButtonType::MainSceneButton_MiniGame] = 26;
        m_buttonHoverTexture[MainSceneButtonType::MainSceneButton_MiniGame] = ResVars::getInstance().textureRes.getTextureFrom("reanim/SelectorScreen_Survival_highlight.png");
        m_jumpSceneType[MainSceneButtonType::MainSceneButton_MiniGame] = SceneType::Scene_WinScene;
    }
    /****************************************************************************/
    {
        SDL_FPoint base_point{ 410.0f, 257.5f };
        std::vector<SDL_FPoint> points{
            SDL_FPoint{  (base_point.x + 7.0f),  (base_point.y + 4.0f) },
            SDL_FPoint{  (base_point.x + 5.0f),  (base_point.y + 58.0f) },
            SDL_FPoint{  (base_point.x + 277.0f),  (base_point.y + 52.0f) },
            SDL_FPoint{  (base_point.x + 265.0f),  (base_point.y + 119.0f) }
        };
        std::vector<int> idx{ 0,1,2,1,3,2 };
        m_buttonRange[MainSceneButtonType::MainSceneButton_Challenge] = std::make_unique<wsTriangles>(points, idx);
        m_buttonHoverOffset[MainSceneButtonType::MainSceneButton_Challenge] = SDL_FPoint{ 3.0f, 3.0f };
        m_buttonTrackIdx[MainSceneButtonType::MainSceneButton_Challenge] = 28;
        m_buttonHoverTexture[MainSceneButtonType::MainSceneButton_Challenge] = ResVars::getInstance().textureRes.getTextureFrom("reanim/SelectorScreen_Challenges_highlight.png");
        m_jumpSceneType[MainSceneButtonType::MainSceneButton_Challenge] = SceneType::Scene_SelectCardScene;
    }
    /****************************************************************************/
    {
        SDL_FPoint base_point{ 413.0f, 328.0f };
        std::vector<SDL_FPoint> points{
            SDL_FPoint{  (base_point.x + 9.0f),  (base_point.y + 4.0f) },
            SDL_FPoint{  (base_point.x + 9.0f),  (base_point.y + 55.0f) },
            SDL_FPoint{  (base_point.x + 260.0f),  (base_point.y + 59.0f) },
            SDL_FPoint{  (base_point.x + 251.0f),  (base_point.y + 116.0f) } };
        std::vector<int> idx{ 0,1,2,1,3,2 };
        m_buttonRange[MainSceneButtonType::MainSceneButton_Survival] = std::make_unique<wsTriangles>(points, idx);
        m_buttonHoverOffset[MainSceneButtonType::MainSceneButton_Survival] = SDL_FPoint{ 3.0f, 3.0f };
        m_buttonTrackIdx[MainSceneButtonType::MainSceneButton_Survival] = 30;
        m_buttonHoverTexture[MainSceneButtonType::MainSceneButton_Survival] = ResVars::getInstance().textureRes.getTextureFrom("reanim/SelectorScreen_vasebreaker_highlight.png");
        m_jumpSceneType[MainSceneButtonType::MainSceneButton_Survival] = SceneType::Scene_SelectCardScene;
    }

}

SceneType MainScene::getType()
{
    return SceneType::Scene_MainScene;
}

int MainScene::enterScene()
{
    SDL_Log("enter main scene\n");
    // 相机位置
    CoreVars::getInstance().camera.setPosition(0.0f, 0.0f);
    // 初始化动画
    setAnimState(MainSceneState::EnterScene);
    // 初始化按钮状态
    m_hoverButtonIdx = MainSceneButtonType::MainScene_MaxButtonNum;
    m_clickButtonIdx = MainSceneButtonType::MainScene_MaxButtonNum;
    for (int button_idx = MainSceneButtonType::MainSceneButton_StartAdventure; button_idx < MainSceneButtonType::MainScene_MaxButtonNum; button_idx++)
    {
        m_animPlayer.setTrackTexture(m_buttonTrackIdx[button_idx], nullptr);
    }
    return 0;
}

SceneType MainScene::handleEvent(SDL_Event& event)
{
    // 动画播放完成后才开始处理事件
    if (m_state == MainSceneState::EnterScene) return SceneType::Scene_MaxSceneIdx;

    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN: {
        // 没有一个处于按下状态的按钮 同时鼠标放置在了一个按钮上
        if (!isValidButton(m_clickButtonIdx) && isValidButton(m_hoverButtonIdx))
        {
            m_clickButtonIdx = m_hoverButtonIdx;
        }
        break;
    }
    case SDL_MOUSEBUTTONUP: {
        SceneType rt_scene = SceneType::Scene_MaxSceneIdx;
        if (isValidButton(m_clickButtonIdx) && m_hoverButtonIdx == m_clickButtonIdx)
        {
            rt_scene = m_jumpSceneType[m_clickButtonIdx];
        }
        m_clickButtonIdx = MainSceneButtonType::MainScene_MaxButtonNum;
        return rt_scene;
    }
    case SDL_MOUSEMOTION: {
        // 遍历按钮 找当前 hover 的按钮
        int button_idx = MainSceneButtonType::MainSceneButton_StartAdventure;
        for (button_idx = MainSceneButtonType::MainSceneButton_StartAdventure; button_idx < MainSceneButtonType::MainScene_MaxButtonNum; button_idx++)
        {
            if (m_buttonRange[button_idx]->isInShape(event.motion.x, event.motion.y))
            {
                break;
            }
        }
        // 原来 hover 着一个按钮 需要 dehover 更换了按钮
        if (isValidButton(m_hoverButtonIdx))
        {
            m_animPlayer.setTrackTexture(m_buttonTrackIdx[m_hoverButtonIdx], nullptr);
            m_hoverButtonIdx = MainSceneButtonType::MainScene_MaxButtonNum;
        }
        // 如果当前 hover 了一个按钮, 变换形态
        if (isValidButton(button_idx))
        {
            m_animPlayer.setTrackTexture(m_buttonTrackIdx[button_idx], m_buttonHoverTexture[button_idx]);
            m_hoverButtonIdx = button_idx;
        }
        break;
    }
    default:
        break;
    }
    return SceneType::Scene_MaxSceneIdx;
}

SceneType MainScene::updateScene()
{
    m_animPlayer.updatePlayingFrameIdx();
    switch (m_state)
    {
    case MainSceneState::EnterScene:
        if (m_animPlayer.isPlayEnd(14))
        {
            setAnimState(MainSceneState::WoodDrop);
        }
        break;
    case MainSceneState::WoodDrop:
        if (m_animPlayer.isPlayEnd(44))
        {
            setAnimState(MainSceneState::Idle);
        }
        break;
    default:
        break;
    }

    return SceneType::Scene_MaxSceneIdx;
}

int MainScene::exitScene()
{
    SDL_Log("exit main scene\n");
    return 0;
}

int MainScene::renderScene()
{
    // render();
    // 背景 
    m_animPlayer.renderTracks({ 14, 21, 22, 23 });
    // 按钮阴影 31/33
    m_animPlayer.renderTracks({ 25, 27, 29, 33 });
    // 按钮 32/34
    for (int button_idx = MainSceneButtonType::MainSceneButton_StartAdventure; button_idx < MainSceneButtonType::MainScene_MaxButtonNum; button_idx++)
    {
        if (button_idx == m_clickButtonIdx) m_animPlayer.renderTrack(m_buttonTrackIdx[button_idx], m_buttonHoverOffset[button_idx]);
        else m_animPlayer.renderTrack(m_buttonTrackIdx[button_idx]);
    }
    // 草
    m_animPlayer.renderTracks({ 35, 36, 37, 38, 39, 40, 41 });
    // 花
    m_animPlayer.renderTracks({ 42, 43, 44 });
    // 木板
    if (m_state != MainSceneState::EnterScene) m_animPlayer.renderTracks({ 45, 46, 47 });
#ifndef NDEBUG
    // 渲染按钮的点击范围
    for (int button_idx = MainSceneButtonType::MainSceneButton_StartAdventure; button_idx < MainSceneButtonType::MainScene_MaxButtonNum; button_idx++)
    {
        m_buttonRange[button_idx]->renderShape();
    }
#endif
    return 0;
}

MainScene::~MainScene()
{}

int MainScene::setAnimState(MainSceneState to_state)
{
    if (m_state == to_state) return 0;
    switch (to_state)
    {
    case MainSceneState::EnterScene:
        m_animPlayer.setPlayingTrack(
            { 14, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 },
            0
        );
        m_animPlayer.restartTrack();
        break;
    case MainSceneState::WoodDrop:
        m_animPlayer.setPlayingTrack(
            { 14, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 },
            1
        );
        m_animPlayer.restartTrack();
        break;
    case MainSceneState::Idle:
        m_animPlayer.setPlayingTrack(
            { 14, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 },
            { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  3,  3,  3,  3,  3,  3,  3,  1,  1,  1,  2,  2,  2 }
        );
        m_animPlayer.restartTrack();
        break;
    default:
        break;
    }
    m_state = to_state;
    return 0;
}