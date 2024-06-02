#include "Resource/AnimLoader.h"
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"
AnimFrame::AnimFrame(
    int m_f,
    SDL_Texture* tex,
    float x, float y,
    float sx, float sy,
    float kx, float ky,
    float alpha
) :
    m_f(m_f),
    m_x(x), m_y(y),
    m_sx(sx), m_sy(sy),
    m_kx(kx), m_ky(ky),
    m_alpha(alpha),
    m_texture(tex),
    m_scale(1.0f),
    m_point{ SDL_FPoint{0.0f, 0.0f},SDL_FPoint{0.0f, 0.0f},SDL_FPoint{0.0f, 0.0f},SDL_FPoint{0.0f, 0.0f} },
    m_enableRefer(false),
    m_anchorPoint(SDL_FPoint{ 0.0f, 0.0f })
{
    m_referPoint.clear();
}

AnimFrame::AnimFrame(const AnimFrame& other) :
    m_f(other.m_f),
    m_x(other.m_x), m_y(other.m_y),
    m_sx(other.m_sx), m_sy(other.m_sy),
    m_kx(other.m_kx), m_ky(other.m_ky),
    m_alpha(other.m_alpha),
    m_texture(other.m_texture),
    m_scale(other.m_scale),
    m_enableRefer(other.m_enableRefer),
    m_anchorPoint(other.m_anchorPoint)
{
    for (int i = 0;i < 4;i++)
    {
        m_point[i] = other.m_point[i];
    }
    if (m_enableRefer)
    {
        m_referPoint = other.m_referPoint;
    }
    else
    {
        m_referPoint.clear();
    }
}

void AnimFrame::caculateVertex(float scale)
{
    if (nullptr == m_texture) return;
    m_scale = scale;
    /** 变换矩阵
     *  sx*cos(kx)  sy*-sin(ky)  x
     *  sx*sin(kx)  sy*cos(ky)   y
     *  0           0            1
     */
    float T_00 = m_scale * m_sx * cosf(m_kx);
    float T_01 = m_scale * m_sy * -sinf(m_ky);
    float T_02 = m_scale * m_x;
    float T_10 = m_scale * m_sx * sinf(m_kx);
    float T_11 = m_scale * m_sy * cosf(m_ky);
    float T_12 = m_scale * m_y;
    int tex_width, tex_height;
    SDL_QueryTexture(m_texture, NULL, NULL, &tex_width, &tex_height);
    m_point[0] = SDL_FPoint{ T_00 * 0 + T_01 * 0 + T_02, T_10 * 0 + T_11 * 0 + T_12 };
    m_point[1] = SDL_FPoint{ T_00 * tex_width + T_01 * 0 + T_02, T_10 * tex_width + T_11 * 0 + T_12 };
    m_point[2] = SDL_FPoint{ T_00 * 0 + T_01 * tex_height + T_02, T_10 * 0 + T_11 * tex_height + T_12 };
    m_point[3] = SDL_FPoint{ T_00 * tex_width + T_01 * tex_height + T_02, T_10 * tex_width + T_11 * tex_height + T_12 };
}

int AnimFrame::transferPoint(SDL_FPoint& point)
{
    if (m_f < 0) return -1;
    /** 变换矩阵
     *  sx*cos(kx)  sy*-sin(ky)  x
     *  sx*sin(kx)  sy*cos(ky)   y
     *  0           0            1
     */
    float T_00 = m_scale * m_sx * cosf(m_kx);
    float T_01 = m_scale * m_sy * -sinf(m_ky);
    float T_02 = m_scale * m_x;
    float T_10 = m_scale * m_sx * sinf(m_kx);
    float T_11 = m_scale * m_sy * cosf(m_ky);
    float T_12 = m_scale * m_y;
    point = SDL_FPoint{ T_00 * point.x + T_01 * point.y + T_02, T_10 * point.x + T_11 * point.y + T_12 };
    return 0;
}

int AnimFrame::setAnchorPoint(const SDL_FPoint& point)
{
    if (m_enableRefer) return -1;
    m_enableRefer = true;
    m_anchorPoint = point;
    transferPoint(m_anchorPoint);
    return 0;
}

int AnimFrame::addReferPoint(const SDL_FPoint& point)
{
    m_referPoint.push_back(point);
    transferPoint(m_referPoint.back());
    return m_referPoint.size() - 1;
}

SDL_FPoint AnimFrame::caculateOffset(const SDL_FPoint& anchorPoint, int referIdx)
{
    if (referIdx < 0 ||
        referIdx >= m_referPoint.size())
    {
        return SDL_FPoint{ 0.0f, 0.0f };
    }
    return SDL_FPoint{ m_referPoint[referIdx].x - anchorPoint.x, m_referPoint[referIdx].y - anchorPoint.y };
}

AnimTrack::AnimTrack(const std::string& name) :
    m_name(name),
    m_referTrackIdx(0),
    m_referPointIdx(0)
{
    m_frames.clear();
}

int AnimTrack::AddFrame(const AnimFrame& new_frame)
{
    m_frames.push_back(new_frame);
    return m_frames.size();
}

void AnimTrack::scaleFrames(float scale)
{
    for (auto& frame : m_frames)
    {
        frame.caculateVertex(scale);
    }
}

int AnimTrack::renderTrack(
    const SDL_FPoint& dst_point,
    int real_frame_idx,
    SDL_Texture* alter_texture,
    Uint8 mask_r, Uint8 mask_g, Uint8 mask_b
)
{
    if (real_frame_idx < 0 || real_frame_idx >= m_frames.size()) return -1;
    if (-1 == m_frames[real_frame_idx].m_f || (nullptr == m_frames[real_frame_idx].m_texture && nullptr == alter_texture)) return -1;
    // 优先使用参数中的纹理
    SDL_Texture* render_texture = (nullptr != alter_texture) ? alter_texture : m_frames[real_frame_idx].m_texture;

    Uint8 TEX_ALPHA = 255 * m_frames[real_frame_idx].m_alpha;

    SDL_Vertex vertices[] = {
        SDL_Vertex {
            {m_frames[real_frame_idx].m_point[0].x + dst_point.x,
            m_frames[real_frame_idx].m_point[0].y + dst_point.y},
            {mask_r, mask_g, mask_b, TEX_ALPHA}, {0, 0} },
        SDL_Vertex {
            {m_frames[real_frame_idx].m_point[1].x + dst_point.x,
            m_frames[real_frame_idx].m_point[1].y + dst_point.y},
            {mask_r, mask_g, mask_b, TEX_ALPHA}, {1, 0} },
        SDL_Vertex {
            {m_frames[real_frame_idx].m_point[2].x + dst_point.x,
             m_frames[real_frame_idx].m_point[2].y + dst_point.y},
            {mask_r, mask_g, mask_b, TEX_ALPHA}, {0, 1} },
        SDL_Vertex {
            {m_frames[real_frame_idx].m_point[3].x + dst_point.x,
            m_frames[real_frame_idx].m_point[3].y + dst_point.y},
            {mask_r, mask_g, mask_b, TEX_ALPHA}, {1, 1} }
    };
    // static float last_frame_X = 0.0f;
    // if (debug_msg && last_frame_X != m_frames[real_frame_idx].m_point[0].x + dst_point.x)
    // {
    //     SDL_Log("frame x: %f, frmae y: %f, off_X: %f, cacu_x: %f, dst_x: %f\n",
    //         m_frames[real_frame_idx].m_point[0].x + dst_point.x,
    //         m_frames[real_frame_idx].m_point[0].x + dst_point.y,
    //         m_frames[real_frame_idx].m_point[0].x + dst_point.x - last_frame_X,
    //         m_frames[real_frame_idx].m_point[0].x,
    //         dst_point.x
    //     );
    //     last_frame_X = m_frames[real_frame_idx].m_point[0].x + dst_point.x;
    // }
    int index[] = { 0, 2, 1, 1, 2, 3 };
    return SDL_RenderGeometry(CoreVars::getInstance().renderer, render_texture, vertices, 4, index, 6);
}

int AnimTrack::renderTrack(
    const SDL_FPoint& dst_point,
    int real_frame_idx,
    Uint8 mask_a
)
{
    if (real_frame_idx < 0 || real_frame_idx >= m_frames.size()) return -1;
    if (-1 == m_frames[real_frame_idx].m_f || nullptr == m_frames[real_frame_idx].m_texture) return -1;

    SDL_Vertex vertices[] = {
        SDL_Vertex {
            {m_frames[real_frame_idx].m_point[0].x + dst_point.x,
            m_frames[real_frame_idx].m_point[0].y + dst_point.y},
            {255, 255, 255, mask_a}, {0, 0} },
        SDL_Vertex {
            {m_frames[real_frame_idx].m_point[1].x + dst_point.x,
            m_frames[real_frame_idx].m_point[1].y + dst_point.y},
            {255, 255, 255, mask_a}, {1, 0} },
        SDL_Vertex {
            {m_frames[real_frame_idx].m_point[2].x + dst_point.x,
             m_frames[real_frame_idx].m_point[2].y + dst_point.y},
            {255, 255, 255, mask_a}, {0, 1} },
        SDL_Vertex {
            {m_frames[real_frame_idx].m_point[3].x + dst_point.x,
            m_frames[real_frame_idx].m_point[3].y + dst_point.y},
            {255, 255, 255, mask_a}, {1, 1} }
    };
    int index[] = { 0, 2, 1, 1, 2, 3 };
    return SDL_RenderGeometry(CoreVars::getInstance().renderer, m_frames[real_frame_idx].m_texture, vertices, 4, index, 6);
}

AnimLoader::AnimLoader(
    const std::string& reanim_path,
    float anim_scale
) :
    m_reanimFilePath(reanim_path),
    m_fps(0.0f),
    m_anim_num(0)
{
    std::string file = "";
    size_t cur = 0; // 当前文件指针
    std::fstream reanim_file(m_reanimFilePath);
    if (reanim_file.is_open())
    {
        // 读入所有内容
        char ch;
        while (reanim_file >> ch) { file += ch; }
        // 读取 fps 标签
        if ("fps" == GetTag(file, cur))
        {
            m_fps = std::stof(GetContent(file, cur, "fps"));
        }
        // 读取轨道数据
        std::string tag;
        bool in_track = false;
        while (cur < file.length())
        {
            tag = GetTag(file, cur);
            if ("track" == tag)
            {
                if (in_track) { m_fps = 0.0f; SDL_Log("loader error\n"); break; }    // 错误的轨道标签
                in_track = true;
            }
            else if ("name" == tag)
            {
                if (!in_track) { m_fps = 0.0f; SDL_Log("loader error\n");break; }   // 错误的轨道标签
                m_tracks.push_back(AnimTrack(GetContent(file, cur, tag)));
            }
            else if ("/track" == tag)
            {
                if (!in_track) { m_fps = 0.0f; SDL_Log("loader error\n");break; }   // 错误的轨道标签
                in_track = false;
            }
            else if ("t" == tag)
            {
                if (!in_track) { m_fps = 0.0f; SDL_Log("loader error\n");break; }   // 错误的轨道标签
                GetFrame(GetContent(file, cur, tag));   // 解析<t>标签内容并添加新的帧到最新轨道
            }
        }
        reanim_file.close();
    }
    // 计算轨道
    for (auto& t : m_tracks)
    {
        t.scaleFrames(anim_scale);
    }
    // 获取动画数量
    m_anim_num = GetAnimTracks();
}

int AnimLoader::Attach(int track_id, const SDL_FPoint& point, int ref_track_id, const SDL_FPoint& ref_point)
{
    // 越界检查
    if (track_id < 0 || track_id >= m_tracks.size()) return -1;
    if (ref_track_id < 0 || ref_track_id >= m_tracks.size()) return -1;
    // 设置锚点
    for (auto& frame : m_tracks[track_id].m_frames)
    {
        if (-1 == frame.setAnchorPoint(point)) return -1;
    }
    // 设置关联点
    int refer_p_idx;
    for (auto& frame : m_tracks[ref_track_id].m_frames)
    {
        refer_p_idx = frame.addReferPoint(ref_point);
    }
    m_tracks[track_id].m_referTrackIdx = ref_track_id;
    m_tracks[track_id].m_referPointIdx = refer_p_idx;
    return 0;
}

int AnimLoader::GetFrame(const std::string& _reanim_str)
{
    std::string reanim_str = _reanim_str;
    size_t start = 0, end = 0;
    std::string tag, content;
    // 读取帧数据
    AnimFrame new_frame;
    if (!m_tracks.back().m_frames.empty()) new_frame = m_tracks.back().m_frames.back(); // 复制最后一帧
    start = 0; end = 0;
    while (end + 1 < reanim_str.length())
    {
        // 获取标签
        start = reanim_str.find_first_of('<', end);
        end = reanim_str.find_first_of('>', start);
        tag = reanim_str.substr(start + 1, end - start - 1);
        // 获取内容
        start = end + 1;
        end = reanim_str.find_first_of('<', start);
        content = reanim_str.substr(start, end - start);
        // 验证反标签
        start = end;
        end = reanim_str.find_first_of('>', start);
        if (reanim_str.substr(start + 2, end - start - 2) != tag) return -1;    // 反标签出错
        if ("i" == tag)
        {
            new_frame.SetImg(ResVars::getInstance().textureRes.getReanimTexture(content));
        }
        else if ("f" == tag)
        {
            new_frame.SetF(std::stoi(content));
        }
        else if ("x" == tag) new_frame.SetX(std::stof(content));
        else if ("y" == tag) new_frame.SetY(std::stof(content));
        else if ("sx" == tag) new_frame.SetSX(std::stof(content));
        else if ("sy" == tag) new_frame.SetSY(std::stof(content));
        else if ("kx" == tag) new_frame.SetKX(std::stof(content) * M_PI / 180.0f);
        else if ("ky" == tag) new_frame.SetKY(std::stof(content) * M_PI / 180.0f);
        else if ("a" == tag) new_frame.SetAlpha(std::stof(content));
    }
    m_tracks.back().m_frames.push_back(new_frame);
    return m_tracks.back().m_frames.size();
}

std::string AnimLoader::PeekTag(const std::string& _file, size_t _cur)
{
    if (_file[_cur] != '<')
    {
        SDL_Log("Get ERROR: not the start position of a tag\n");
        return "";
    }
    size_t end = _file.find_first_of('>', _cur);
    return _file.substr(_cur + 1, end - _cur - 1);
}

std::string AnimLoader::GetTag(const std::string& _file, size_t& _cur)
{
    size_t cur = _cur;
    if (_file[_cur] != '<')
    {
        SDL_Log("Get ERROR: not the start position of a tag\n");
        return "";
    }
    _cur = _file.find_first_of('>', cur) + 1;
    return _file.substr(cur + 1, _cur - cur - 2);
}

size_t AnimLoader::FindEndTag(const std::string& file, size_t start_pos, const std::string& tag)
{
    std::string end_tag = "/" + tag;
    size_t temp_pos = start_pos;
    while (1)
    {
        temp_pos = file.find_first_of('<', start_pos);
        start_pos = temp_pos;
        if (end_tag == GetTag(file, temp_pos)) return start_pos;
        else start_pos = temp_pos;
    }
}

std::string AnimLoader::GetContent(const std::string& _file, size_t& _cur, const std::string& tag)
{
    size_t cur = _cur;
    _cur = FindEndTag(_file, cur, tag) + 3 + tag.length();
    return _file.substr(cur, _cur - cur - 3 - tag.length());
}

int AnimLoader::GetAnimTracks()
{
    for (auto& track : m_tracks)
    {
        if ("anim_" != track.m_name.substr(0, 5)) continue;
        int begin = -1, end = -1;
        for (int i = 0;i < track.m_frames.size();i++)
        {
            if (track.m_frames[i].m_f == 0 && begin == -1) begin = i;
            if (track.m_frames[i].m_f == -1 && begin != -1)
            {
                end = i - 1;
                break;
            }
        }
        if (-1 == end) end = track.m_frames.size() - 1;
        m_animTracks.push_back(Control_Track{ track.m_name, begin, end, end - begin + 1 });
        // std::string out_msg = track.m_name + ": " + std::to_string(begin) + ", " + std::to_string(end);
        // SDL_Log(out_msg.c_str());
    }
    // 不存在标识动画的轨道, 默认全部播放
    if (0 == m_animTracks.size() && 0 != m_tracks.size())
    {
        m_animTracks.push_back(Control_Track{ "Full", 0, static_cast<int>(m_tracks[0].m_frames.size()) - 1, static_cast<int>(m_tracks[0].m_frames.size()) });
        for (auto& track : m_tracks)
        {
            for (auto& frame : track.m_frames)
            {
                frame.m_f = 0;
            }
        }
    }
    // SDL_Log("\n");
    return m_animTracks.size();
}