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
    uint64_t m_rmCoolMilliSecond;   // 距离冷却完成所需的时间
    bool m_endble;                  // 可选择
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

    // 清空卡槽
    void clearCardSlot();
    // 初始化卡池中的植物类型
    // 在再次进入选卡界面时调用
    // 因为选卡仅更改卡片植物类型
    void resetCardPool();
    // 重新开始冷却卡槽
    void restartCoolDown();
    // 更新卡槽中的冷却时间等信息
    void updateCardInSlot();
    
    // 拿起一张卡片 卡片在卡槽中变暗
    // 返回卡片的植物类型
    // 失败返回 MAX
    PlantType pickupCard(int card_slot_idx);
    // 放回一张卡片到卡槽中
    // 成功返回 0 ,失败返回 -1
    int putbackCard(int card_slot_idx);
    // 将卡片对应的植物安置到场景中
    // 刷新使用时间 进入冷却
    // 扣除阳光
    int settleCard(int card_slot_idx);

    // 卡片在卡槽与卡池之间移动
    int slot2pool(int card_slot_idx);
    int pool2slot(int card_pool_idx);

    // 根据坐标返回卡槽编号
    // 返回 -1 代表该坐标不在任何卡槽卡片范围内
    int getSlotIdx(int x, int y);
    // 根据坐标返回卡池编号
    // 返回 -1 代表该坐标不在任何卡池卡片范围内
    int getPoolIdx(int x, int y);

    // 判断卡槽是否填满
    // 填满 or 所有的卡都在卡槽中
    inline bool isFullSlot()
    {
        return m_cardSlotNum == m_cardInSlot.size() || m_cardInPool.size() == m_cardInSlot.size();
    }

    int renderCardSlot();       // 绘制卡槽
    int renderCardCoolDown();   // 绘制游戏过程中卡槽的冷却信息
    int renderCardPool();       // 绘制卡池

    void releaseManagers();

    ~CardManager();

protected:
    inline bool hasCardInSlot(int slot_idx)
    {
        return m_cardInSlot[slot_idx].m_plantType != PlantType::MaxPlantType;
    }

    inline bool hasCardInPool(int slot_idx)
    {
        return m_cardInPool[slot_idx].m_plantType != PlantType::MaxPlantType;
    }
};



#endif