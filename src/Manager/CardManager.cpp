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
    m_textureRes(res),
    m_cardPoolBK(nullptr),
    m_cardPoolRange{ 0,0,0,0 },
    m_cardSlotBK(nullptr),
    m_cardSlotRange{ 0,0,0,0 },
    m_cardWidth(50),
    m_cardHeight(70),
    m_poolCardOffsetX(23),
    m_poolCardOffsetY(39),
    m_poolCardSepX(2),
    m_poolCardSepY(2),
    m_poolCardPerRow(8),
    m_slotCardOffsetX(78),
    m_slotCardOffsetY(8),
    m_slotCardSepX(1),
    m_cardSlotNum(7)
{
    m_cardPoolBK = m_textureRes->getTextureFrom("images/SeedChooser_Background.png");
    if (m_cardPoolBK != nullptr)
    {
        SDL_QueryTexture(m_cardPoolBK, NULL, NULL, &m_cardPoolRange.w, &m_cardPoolRange.h);
        m_cardPoolRange.x = 0;
        m_cardPoolRange.y = 600 - m_cardPoolRange.h;
    }
    m_cardSlotBK = m_textureRes->getTextureFrom("images/SeedBank.png");
    if (m_cardSlotBK != nullptr)
    {
        SDL_QueryTexture(m_cardSlotBK, NULL, NULL, &m_cardSlotRange.w, &m_cardSlotRange.h);
        m_cardSlotRange.x = 0;
        m_cardSlotRange.y = 0;
    }
    // 设置卡片在卡池中占据的位置
    int poolCardStartX = m_cardSlotRange.x + m_poolCardOffsetX;
    int poolCardStartY = m_cardSlotRange.y + m_cardSlotRange.h + m_poolCardOffsetY;
    for (int i = 0, x = poolCardStartX, y = poolCardStartY;
        i < PlantType::MaxPlantType;
        i++)
    {
        m_cardRangeInPool.push_back(wsRectangle{ SDL_Rect{x, y, m_cardWidth, m_cardHeight}, m_renderer });
        x += m_cardWidth + m_poolCardSepX;
        if ((i % m_poolCardPerRow) == m_poolCardPerRow - 1)
        {
            x = poolCardStartX;
            y += m_cardHeight + m_poolCardSepY;
        }
    }
    // 设置卡片在卡槽中占据的位置
    for (int i = 0, x = m_slotCardOffsetX, y = m_slotCardOffsetY;
        i < m_cardSlotNum;
        i++)
    {
        m_cardRangeInSlot.push_back(wsRectangle{ SDL_Rect{x, y, m_cardWidth, m_cardHeight}, m_renderer });
        x += m_cardWidth + m_slotCardSepX;
    }
    // 初始化卡池中卡片的信息
    m_cardInPool.resize(PlantType::MaxPlantType);
    XmlLoader loader;
    if (0 != loader.Import("resource/card_data.xml"))
    {
        SDL_Log("CardManager can't load resource/card_data.xml");
    }
    auto children = loader.m_root->getChildren();
    for (auto child : children)
    {
        CardNode new_node{ PlantType::MaxPlantType,0,0,0 };
        std::string content;
        content = child->getAttr("type");
        if (content != "")
        {
            int type = std::stoi(content);
            if (type >= PlantType::MaxPlantType || type < 0)
            {
                SDL_Log("unknown card in card_data.xml\nName: %s, Type: %s", child->getName(), content);
                continue;
            }
            new_node.m_plantType = static_cast<PlantType>(type);
        }
        content = child->getAttr("sun_cost");
        if (content != "") new_node.m_sunCost = std::stoi(content);
        content = child->getAttr("cool_ms");
        if (content != "") new_node.m_coolMilliSecond = std::stoi(content);
        new_node.m_rmCoolMilliSecond = new_node.m_coolMilliSecond;
        new_node.m_endble = true;
        m_cardInPool[new_node.m_plantType] = new_node;
    }
    // 初始化卡槽中的卡片
    clearCardSlot();
}

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

void CardManager::clearCardSlot()
{
    m_cardInSlot.clear();
}

void CardManager::restartCoolDown()
{
    for (auto& card : m_cardInSlot)
    {
        card.m_rmCoolMilliSecond = card.m_coolMilliSecond;
    }
}

void CardManager::updateCardInSlot()
{
    for (auto& card : m_cardInSlot)
    {
        if (card.m_rmCoolMilliSecond <= m_timer->getDeltaTime()) card.m_rmCoolMilliSecond = 0;
        else card.m_rmCoolMilliSecond -= m_timer->getDeltaTime();
    }
}

PlantType CardManager::pickupCard(int card_slot_idx)
{
    if (card_slot_idx >= 0
        && card_slot_idx < m_cardInSlot.size()
        && m_cardInSlot[card_slot_idx].m_endble
        && 0 == m_cardInSlot[card_slot_idx].m_rmCoolMilliSecond)
    {
        m_cardInSlot[card_slot_idx].m_endble = false;
        SDL_Log("pick a card: %d\n", card_slot_idx);
        return m_cardInSlot[card_slot_idx].m_plantType;
    }
    return PlantType::MaxPlantType;
}

int CardManager::putbackCard(int card_slot_idx)
{
    if (card_slot_idx >= 0
        && card_slot_idx < m_cardInSlot.size()
        && !m_cardInSlot[card_slot_idx].m_endble)
    {
        m_cardInSlot[card_slot_idx].m_endble = true;
        SDL_Log("put back a card %d\n", card_slot_idx);
        return 0;
    }
    return -1;
}

int CardManager::settleCard(int card_slot_idx)
{
    if (card_slot_idx >= 0
        && card_slot_idx < m_cardInSlot.size())
    {
        m_cardInSlot[card_slot_idx].m_endble = true;
        m_cardInSlot[card_slot_idx].m_rmCoolMilliSecond = m_cardInSlot[card_slot_idx].m_coolMilliSecond;
        return card_slot_idx;
    }
    return -1;
}

int CardManager::slot2pool(int card_slot_idx)
{
    if (!hasCardInSlot(card_slot_idx)) return -1;
    PlantType type = m_cardInSlot[card_slot_idx].m_plantType;
    m_cardInPool[type].m_plantType = type;
    m_cardInSlot.erase(m_cardInSlot.begin() + card_slot_idx);
    return 0;
}

int CardManager::pool2slot(int card_pool_idx)
{
    if (!hasCardInPool(card_pool_idx)) return -1;
    m_cardInSlot.push_back(m_cardInPool[card_pool_idx]);
    m_cardInPool[card_pool_idx].m_plantType = PlantType::MaxPlantType;
    return 0;
}

int CardManager::getSlotIdx(int x, int y)
{
    int card_idx = (x - m_slotCardOffsetX) / (m_cardWidth + m_slotCardSepX);
    if (card_idx >= 0 && card_idx < m_cardInSlot.size()
        && m_cardRangeInSlot[card_idx].isInShape(x, y))
    {
        return card_idx;
    }
    return -1;
}

int CardManager::getPoolIdx(int x, int y)
{
    x -= m_poolCardOffsetX;
    y -= (m_poolCardOffsetY + m_cardSlotRange.h);
    if (x < 0 || y < 0) return -1;
    x /= (m_cardWidth + m_poolCardSepX);
    y /= (m_cardHeight + m_poolCardOffsetY);
    int card_idx = x * m_poolCardPerRow + y;
    if (card_idx < m_cardInPool.size()) return card_idx;
    return -1;
}

int CardManager::renderCardSlot()
{
    SDL_RenderCopy(m_renderer, m_cardSlotBK, NULL, &m_cardSlotRange);
    for (int i = 0; i < m_cardInSlot.size();i++)
    {
        if (m_cardInSlot[i].m_plantType == PlantType::MaxPlantType) break;
        SDL_RenderCopy(m_renderer, m_plantCardTexture[m_cardInSlot[i].m_plantType], NULL, &m_cardRangeInSlot[i].m_range);
    }
    // 显示可点击范围
    for (auto i : m_cardRangeInSlot)
    {
        i.renderShape();
    }
    return 0;
}

int CardManager::renderCardCoolDown()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 75);
    for (int i = 0; i < m_cardInSlot.size();i++)
    {
        if (m_cardInSlot[i].m_plantType == PlantType::MaxPlantType) break;
        if (0 == m_cardInSlot[i].m_rmCoolMilliSecond) continue;
        SDL_Rect mask_range{
            m_cardRangeInSlot[i].m_range.x,
            m_cardRangeInSlot[i].m_range.y,
            m_cardRangeInSlot[i].m_range.w,
            m_cardRangeInSlot[i].m_range.h* (int)(m_cardInSlot[i].m_rmCoolMilliSecond) / (int)(m_cardInSlot[i].m_coolMilliSecond)
        };
        SDL_RenderFillRect(m_renderer, &mask_range);
    }
    return 0;
}

int CardManager::renderCardPool()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 75);
    SDL_RenderCopy(m_renderer, m_cardPoolBK, NULL, &m_cardPoolRange);
    for (int i = 0; i < PlantType::MaxPlantType;i++)
    {
        if(m_cardInPool[i].m_plantType == PlantType::MaxPlantType) continue;
        SDL_RenderCopy(m_renderer, m_plantCardTexture[i], NULL, &m_cardRangeInPool[i].m_range);
        if (!m_cardInPool[i].m_endble)
        {
            SDL_RenderFillRect(m_renderer, &m_cardRangeInPool[i].m_range);
        }
    }
    // 显示可点击范围
    for (auto i : m_cardRangeInPool)
    {
        i.renderShape();
    }
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
