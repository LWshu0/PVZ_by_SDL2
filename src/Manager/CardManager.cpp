#include "Manager/CardManager.h"
#include "Manager/PlantManager.h"

CardManager::CardManager(
    SDL_Renderer* renderer,
    std::shared_ptr<Timer> timer,
    std::shared_ptr<Camera> camera,
    std::shared_ptr<TextureRes> res
) :
    m_renderer(renderer),
    m_timer(timer),
    m_camera(camera),
    m_textureRes(res)
{}

int CardManager::initilizeManagers(
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<PlantManager> plantManager
)
{
    m_mapManager = mapManager;
    m_plantManager = plantManager;
    if (m_plantManager == nullptr) return -1;
    // 初始化纹理数组为最大 plant 数量
    m_plantCardTexture.resize(PlantType::MaxPlantType);
    m_plantImageTexture.resize(PlantType::MaxPlantType);

    SDL_Texture* card_bk = m_textureRes->getTextureFrom("images/SeedPacket_Larger.png");
    int card_w = 0, card_h = 0;
    SDL_QueryTexture(card_bk, NULL, NULL, &card_w, &card_h);
    for (int i = PlantType::PlantPeaShooter1; i < PlantType::MaxPlantType; i++)
    {
        // 创建植物预览图片纹理
        // 设置植物状态 获取需要的纹理大小
        std::shared_ptr<PlantObject> plant = plantManager->getPlantTemplate(i);
        plant->changeToStatic();
        float width, height;
        plant->getAnimRange(width, height);
        // 根据上述大小创建纹理
        SDL_Texture* plant_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        SDL_SetTextureBlendMode(plant_texture, SDL_BLENDMODE_BLEND);
        // 清空纹理为全透明
        SDL_SetRenderTarget(m_renderer, plant_texture);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
        SDL_RenderClear(m_renderer);
        // 渲染植物
        plant->renderToTexture();
        m_plantImageTexture[i] = plant_texture;
        // 创建植物卡片纹理
        SDL_Texture* card_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, card_w, card_h);
        SDL_SetTextureBlendMode(card_texture, SDL_BLENDMODE_BLEND);
        // 清空纹理为全透明
        SDL_SetRenderTarget(m_renderer, card_texture);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
        SDL_RenderClear(m_renderer);
        // 渲染卡片背景
        SDL_RenderCopy(m_renderer, card_bk, NULL, NULL);
        // 尽量把植物渲染到卡片的中间位置
        // 0.14 - 0.71
        // 0.1 - 0.9
        SDL_FRect dst_rect{ card_w * 0.1f, card_h * 0.14f, card_w * 0.8f, card_h * 0.57f };
        if (dst_rect.w / dst_rect.h >= width / height)
        {
            float new_width = dst_rect.h * width / height;
            dst_rect.x += (dst_rect.w - new_width) / 2;
            dst_rect.w = new_width;
        }
        else
        {
            float new_height = dst_rect.w * height / width;
            dst_rect.y += (dst_rect.h - new_height);
            dst_rect.h = new_height;
        }
        SDL_RenderCopyF(m_renderer, plant_texture, NULL, &dst_rect);
        m_plantCardTexture[i] = card_texture;
    }
    SDL_SetRenderTarget(m_renderer, NULL);
    return 0;
}

int CardManager::renderCard()
{
    SDL_Rect dst_rect{ 0, 0, 100, 200 };
    SDL_Rect dst_rect2{ 200, 0, 100, 200 };
    SDL_QueryTexture(m_plantImageTexture[0], NULL, NULL, &dst_rect.w, &dst_rect.h);
    SDL_QueryTexture(m_plantCardTexture[0], NULL, NULL, &dst_rect2.w, &dst_rect2.h);
    SDL_RenderCopy(m_renderer, m_plantImageTexture[0], NULL, &dst_rect);
    SDL_RenderCopy(m_renderer, m_plantCardTexture[0], NULL, &dst_rect2);
    return 0;
}

void CardManager::releaseManagers()
{
    m_mapManager = nullptr;
    m_plantManager = nullptr;
}

CardManager::~CardManager()
{
    for (SDL_Texture* tex : m_plantImageTexture)
    {
        if (tex != nullptr) SDL_DestroyTexture(tex);
    }
    for (SDL_Texture* tex : m_plantCardTexture)
    {
        if (tex != nullptr) SDL_DestroyTexture(tex);
    }
}
