#include "Scene/MainScene.h"
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
MainScene::MainScene(
    std::shared_ptr<Timer> timer,
    std::shared_ptr<Camera> camera,
    std::shared_ptr<TextureRes> res,
    std::shared_ptr<AnimLoader> loader
) :
    SceneObject(timer, camera, res),
    AnimPlayer(loader, camera, SDL_FPoint{ 0.0f, 0.0f }, AnimState::R_ANIM1, DamageState::R_Damage1)
{}

int MainScene::changeAnimState(AnimState to_state)
{
    if (m_playingAnimState == to_state) return 0;
    m_playingAnimState = to_state;
    switch (m_playingAnimState)
    {
    case AnimState::R_ANIM1:
        setPlayingTrack(
            { 14, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 },
            0
        );
        restartTrack();
        break;
    case AnimState::R_ANIM2:
        setPlayingTrack(
            { 14, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 },
            1
        );
        restartTrack();
        break;
    case AnimState::R_ANIM3:
        setPlayingTrack(
            { 14, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 },
            { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  3,  3,  3,  3,  3,  3,  3,  1,  1,  1,  2,  2,  2 }
        );
        restartTrack();
        break;
    default:
        break;
    }
    return 0;
}

int MainScene::enterScene()
{
    setPlayingTrack(
        { 14, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 },
        0
    );
    restartTrack();
    return 0;
}

SceneType MainScene::handleEvent(SDL_Event& event)
{

    return SceneType::Scene_MaxSceneIdx;
}

int MainScene::updateScene()
{
    updatePlayingFrameIdx(m_timer->getTime());
    switch (m_playingAnimState)
    {
    case AnimState::R_ANIM1:
        if (isPlayEnd(14))
        {
            changeAnimState(AnimState::R_ANIM2);
        }
        break;
    case AnimState::R_ANIM2:
        if (isPlayEnd(44))
        {
            changeAnimState(AnimState::R_ANIM3);
        }
        break;
    default:
        break;
    }

    return 0;
}

int MainScene::exitScene()
{
    return 0;
}

int MainScene::renderScene()
{
    return render();
}

MainScene::~MainScene()
{}
