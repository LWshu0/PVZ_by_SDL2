#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include <vector>
#include <memory>

#include "SDL.h"
#include "Core/Timer.h"
#include "Core/Camera.h"
#include "Core/TextureRes.h"
#include "Core/UI/wsRectangle.h"
#include "Core/XmlLoader.h"

#include "Plants/PlantObject.h"

class MapManager;
class PlantManager;

struct CardNode { //卡槽卡池节点
    PlantType m_plantType;          // 植物类型
    int m_sunCost;                  // 阳关消耗
    uint64_t m_coolMilliSecond;     // 冷却时间
    // 状态
    uint64_t m_lastUseMilliSecond;  //最后使用时间
};

class CardManager {
protected:
    SDL_Renderer* m_renderer;
    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<TextureRes> m_textureRes;

    // 素材位置参数
    int m_cardWidth;        // 卡片像素宽度
    int m_cardHeight;       // 卡片像素高度
    int m_poolCardOffsetX;  // 卡池中第一张卡片相对于卡池背景偏移的 X 值
    int m_poolCardOffsetY;  // 卡池中第一张卡片相对于卡池背景偏移的 Y 值
    int m_poolCardSepX;     // 卡池中相邻卡片之间在 X 方向上的像素数量
    int m_poolCardSepY;     // 卡池中相邻卡片之间在 Y 方向上的像素数量
    int m_poolCardPerRow;   // 卡池中一行的卡片数量
    int m_slotCardOffsetX;  // 卡槽中第一张卡片相对于卡槽背景偏移的 X 值
    int m_slotCardOffsetY;  // 卡槽中第一张卡片相对于卡槽背景偏移的 Y 值
    int m_slotCardSepX;     // 卡槽中相邻卡片之间在 X 方向上的像素数量

    /* TextureRes 管理不需要释放 */
    SDL_Texture* m_cardPoolBK;  // 卡池背景
    SDL_Rect m_cardPoolRange;   // 卡池范围
    std::vector<wsRectangle> m_cardRangeInPool; // 卡池中卡片的范围
    std::vector<CardNode> m_cardInPool; // 卡池中的卡片
    SDL_Texture* m_cardSlotBK;  // 卡槽背景
    SDL_Rect m_cardSlotRange;   // 卡槽范围
    std::vector<wsRectangle> m_cardRangeInSlot; // 卡槽中卡片的范围
    std::vector<CardNode> m_cardInSlot; // 卡槽中的卡片

    /* card manager 管理需要手动释放 */
    // 植物纹理
    std::vector<SDL_Texture*> m_plantImageTexture;
    // 卡片纹理
    std::vector<SDL_Texture*> m_plantCardTexture;

    // 用户信息
    int m_cardSlotNum;      // 解锁的卡槽数量

    // 关联 manager
    std::shared_ptr<MapManager> m_mapManager;
    std::shared_ptr<PlantManager> m_plantManager;

public:
    CardManager(
        SDL_Renderer* renderer,
        std::shared_ptr<Timer> timer,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<TextureRes> res
    );

    // 关联 manager 初始化 texture
    int initilizeManagers(
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<PlantManager> plantManager
    );

    // 检查卡槽数量是否正确(resize到正确大小) 清空卡槽
    void clearCardSlot();

    int renderCardSlot();
    int renderCardPool();

    void releaseManagers();

    ~CardManager();
};



#endif