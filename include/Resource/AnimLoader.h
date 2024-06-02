#ifndef ANIMLOADER_H
#define ANIMLOADER_H

#include <vector>
#include <memory>
#include <cmath>
#include <string>
#include "SDL.h"
#include "Core/ExtSDL.h"

#include "Resource/TextureRes.h"
#include "Resource/XmlLoader.h"

class AnimFrame {
public:
    /*******************
     *     元数据      *
     ******************/
    int m_f;                    // 可见性 0 代表不可见, 1 代表可见
    float m_x, m_y;             // 纹理相对于物体坐标系的偏移
    float m_sx, m_sy;           // 坐标轴缩放倍数
    float m_kx, m_ky;           // 坐标轴旋转弧度
    float m_alpha;              // 透明度
    SDL_Texture* m_texture;     // 纹理
    float m_scale;              // 额外的缩放因子(非文件读取, 由程序定义)
    // 预计算纹理顶点位置 绘制时只需对顶点进行平移即可绘制到指定的位置
    // 对应纹理坐标为
    // 0 : {0, 0}
    // 1 : {1, 0}
    // 2 : {0, 1}
    // 3 : {1, 1}
    SDL_FPoint m_point[4];
    bool m_enableRefer;
    SDL_FPoint m_anchorPoint;               // 自身的锚点 需要把该锚点连接到另一帧的 referPoint 上
    std::vector<SDL_FPoint> m_referPoint;   // 锚点数组, 其他组件的帧需要参考该帧的点
public:
    AnimFrame(
        int f = 0,
        SDL_Texture* tex = nullptr,
        float x = 0.0f, float y = 0.0f,
        float sx = 1.0f, float sy = 1.0f,
        float kx = 0.0f, float ky = 0.0f,
        float alpha = 1.0f);
    
    AnimFrame(const AnimFrame& other);

    inline void SetF(int f) { m_f = f; }
    inline void SetImg(SDL_Texture* tex) { m_texture = tex; }
    inline void SetX(float x) { m_x = x; }
    inline void SetY(float y) { m_y = y; }
    inline void SetSX(float sx) { m_sx = sx; }
    inline void SetSY(float sy) { m_sy = sy; }
    inline void SetKX(float kx) { m_kx = kx; }
    inline void SetKY(float ky) { m_ky = ky; }
    inline void SetAlpha(float alpha) { m_alpha = alpha; }

    // 计算缩放指定倍数的顶点位置
    void caculateVertex(float scale);
    // 根据该帧的元数据对一个点进行线性变换
    int transferPoint(SDL_FPoint& point);
    // 为该帧设置一个锚点 使得计算偏移时根据锚点计算
    // 锚点不可重复设置
    int setAnchorPoint(const SDL_FPoint& point);
    // 为该帧添加一个参考点
    // 返回参考点在该帧中的下标
    int addReferPoint(const SDL_FPoint& point);
    // 根据参考点计算偏移
    SDL_FPoint caculateOffset(const SDL_FPoint& anchorPoint, int referIdx);
};

// 一条动画轨道 包含一个物体的一个组件动画
class AnimTrack {
public:
    std::string m_name;
    std::vector<AnimFrame> m_frames;
    // 参考轨道标识符(在 AnimPlayer 中定义)
    int m_referTrackIdx;
    // 参考的点在参考帧锚点数组中的下标
    int m_referPointIdx;
public:
    AnimTrack(const std::string& name = "");
    int AddFrame(const AnimFrame& new_frame);
    void scaleFrames(float scale);

    int renderTrack(
        const SDL_FPoint& dst_point,
        int real_frame_idx,
        SDL_Texture* alter_texture = nullptr,
        Uint8 mask_r = 255, Uint8 mask_g = 255, Uint8 mask_b = 255
    );

    int renderTrack(
        const SDL_FPoint& dst_point,
        int real_frame_idx,
        Uint8 mask_a
    );

    ~AnimTrack() {};
};

// 标识动画起始点 一个 reanim 中有多种动画
struct Control_Track {
    std::string name;
    int begin;
    int end;
    int length;
};

// 动画数据转换器 从 reanim 文档信息转为可播放的动画信息
class AnimLoader {
public:
    // .reanim 文件路径
    std::string m_reanimFilePath;
    // 动画默认帧率(由文件指定)
    float m_fps;
    // 动画种类数量
    int m_anim_num;
    // 记录动画名称与起始帧(全闭区间)
    std::vector<Control_Track> m_animTracks;
    // 全部动画轨道数据
    std::vector<AnimTrack> m_tracks;

public:
    AnimLoader(
        const std::string& reanim_path,
        float anim_scale = 1.0f
    );
    
    /**
     *@brief 连接轨道
     *
     * @param _track_id 参考轨道
     * @param _point 参考轨道得锚点
     * @param ref_track_id 被参考轨道
     * @param ref_point 被参考轨道锚点
     * @return int 0 代表 success, -1 代表 fail
     */
    int Attach(int track_id, const SDL_FPoint& point, int ref_track_id, const SDL_FPoint& ref_point);

    ~AnimLoader() {};
protected:
    // 读取reanim内容
    int GetFrame(const std::string& _reanim_str);
    std::string PeekTag(const std::string& _file, size_t _cur);
    std::string GetTag(const std::string& _file, size_t& _cur);
    size_t FindEndTag(const std::string& file, size_t start_pos, const std::string& tag);
    // 返回该标签的内容并将读取位置跳到反标签之后一个字符
    std::string GetContent(const std::string& _file, size_t& _cur, const std::string& tag);

    // 获取标识动画轨道
    int GetAnimTracks();
};

#endif