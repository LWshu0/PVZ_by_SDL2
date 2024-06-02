#include "Scene/WinScene.h"
#include "Manager/CardManager.h"
// 全局单例
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"

WinScene::WinScene() :
    SceneObject(),
    m_bkTexture(nullptr),
    m_buttonIdle(nullptr),
    m_buttonHover(nullptr),
    m_buttonNextTexture(nullptr),
    m_buttonQuitTexture(nullptr),
    m_buttonNextRange(SDL_Rect{ 152, 511, 156, 42 }),
    m_buttonQuitRange(SDL_Rect{ 467, 511, 156, 42 }),
    m_cardRect{350, 140, 100, 140},
    m_buttonFont("data/_BrianneTod16.png", "data/BrianneTod16.txt", SDL_Color{ 255,255,0,255 }),
    m_inforFont("data/_BrianneTod16.png", "data/BrianneTod16.txt", SDL_Color{ 0, 0, 0, 255 })
{
    m_bkTexture = ResVars::getInstance().textureRes.getTextureFrom("images/AwardScreen_Back.jpg");
    m_buttonIdle = ResVars::getInstance().textureRes.getTextureFrom("images/SeedChooser_Button.png");
    m_buttonHover = ResVars::getInstance().textureRes.getTextureFrom("images/SeedChooser_Button_Glow.png");
}

SceneType WinScene::getType()
{
    return SceneType::Scene_WinScene;
}

int WinScene::enterScene()
{
    SDL_Log("enter select card scene\n");
    m_buttonNextTexture = nullptr;
    m_buttonQuitTexture = nullptr;
    // 相机位置
    CoreVars::getInstance().camera.setPosition(0.0f, 0.0f);
    return 0;
}

SceneType WinScene::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) return SceneType::Scene_MainScene;
    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        SDL_Log("mouse button left\n");
        int click_x = CoreVars::getInstance().camera.getClickX(event.button.x);
        int click_y = CoreVars::getInstance().camera.getClickX(event.button.y);
        if (m_buttonNextRange.isInShape(click_x, click_y)) return SceneType::Scene_SelectCardScene;
        else if (m_buttonQuitRange.isInShape(click_x, click_y)) return SceneType::Scene_MainScene;
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
        int motion_x = CoreVars::getInstance().camera.getClickX(event.button.x);
        int motion_y = CoreVars::getInstance().camera.getClickX(event.button.y);
        if (m_buttonNextRange.isInShape(motion_x, motion_y))
        {
            m_buttonNextTexture = m_buttonHover;
            m_buttonQuitTexture = nullptr;
        }
        else if (m_buttonQuitRange.isInShape(motion_x, motion_y))
        {
            m_buttonNextTexture = nullptr;
            m_buttonQuitTexture = m_buttonHover;
        }
        else
        {
            m_buttonNextTexture = nullptr;
            m_buttonQuitTexture = nullptr;
        }
    }
    return SceneType::Scene_MaxSceneIdx;
}

SceneType WinScene::updateScene()
{
    return SceneType::Scene_MaxSceneIdx;
}

int WinScene::exitScene()
{
    SDL_Log("exit select card scene\n");
    return 0;
}

int WinScene::renderScene()
{
    // 背景
    SDL_RenderCopy(CoreVars::getInstance().renderer, m_bkTexture, NULL, NULL);
    // 按钮底
    SDL_RenderCopy(CoreVars::getInstance().renderer, m_buttonIdle, NULL, &m_buttonNextRange.m_range);
    SDL_RenderCopy(CoreVars::getInstance().renderer, m_buttonIdle, NULL, &m_buttonQuitRange.m_range);
    // 按钮字
    m_buttonFont.render("下一关!", m_buttonNextRange.m_range.x + 20, m_buttonNextRange.m_range.y, 1.5f);
    m_buttonFont.render("返回主界面!", m_buttonQuitRange.m_range.x, m_buttonQuitRange.m_range.y, 1.5f);
    m_buttonFont.render("你得到一株新植物!", 240, 20, 2.0f);
    // 发光
    if (m_buttonNextTexture != nullptr)
        SDL_RenderCopy(CoreVars::getInstance().renderer, m_buttonNextTexture, NULL, &m_buttonNextRange.m_range);
    if (m_buttonQuitTexture != nullptr)
        SDL_RenderCopy(CoreVars::getInstance().renderer, m_buttonQuitTexture, NULL, &m_buttonQuitRange.m_range);
    // 卡片
    SDL_RenderCopy(
        CoreVars::getInstance().renderer,
        Managers::getInstance().cardManager->getCardTexture(PlantType::PlantPeaShooter1),
        NULL,
        &m_cardRect
    );
    // 描述
    m_inforFont.render("暂无描述", 324, 393);
    return 0;
}

WinScene::~WinScene()
{}
