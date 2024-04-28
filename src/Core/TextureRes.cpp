#include "Core/TextureRes.h"

TextureRes::TextureRes(
    SDL_Renderer* renderer,
    const std::string& xml_path,
    const std::string& image_path
) :
    m_renderer(renderer),
    m_imageRootPath(image_path)
{
    buildObjName();
    XmlLoader loader;
    if (-1 != loader.Import(xml_path))
    {
        std::string prefix = loader.m_root->getAttr("prefix");
        auto& children = loader.m_root->getChildren();
        for (auto ptr : children)
        {
            m_reanim2real[prefix + ptr->getName()] = ptr->getAttr("path");
        }
    }
}

SDL_Texture* TextureRes::getTextureFrom(const std::string& file_path)
{
    if (m_textures.find(file_path) == m_textures.end())
    {
        SDL_Texture* img_texture = IMG_LoadTexture(m_renderer, file_path.c_str());
        if (img_texture != NULL)
        {
            m_textures[file_path] = img_texture;
        }
        else
        {
            SDL_Log("failed to load: %s\n", file_path.c_str());
            return nullptr;
        }
    }
    return m_textures[file_path];
}

SDL_Texture* TextureRes::getTextureWithMask(const std::string& _file_path, const std::string& _mask_path)
{
    SDL_Surface* surface = nullptr, * mask = nullptr;
    surface = IMG_Load(_file_path.c_str());
    mask = IMG_Load(_mask_path.c_str());
    if (!ToRGBA8888(surface) || !ToRGBA8888(mask))
    {
        SDL_FreeSurface(surface);
        SDL_FreeSurface(mask);
        return nullptr;
    }
    // 锁定表面以获取访问权限
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
    if (SDL_MUSTLOCK(mask)) SDL_LockSurface(mask);
    // 实际数据
    Uint8* pix1 = (Uint8*)surface->pixels;
    Uint8* pix2 = (Uint8*)mask->pixels;
    // 逐位相乘
    size_t length = surface->pitch * surface->h;
    for (size_t i = 0;i < length;i += 4)
    {
        pix1[i] *= (static_cast<float>(pix2[i + 1]) / 255.0f);
        pix1[i + 1] *= (static_cast<float>(pix2[i + 1]) / 255.0f);
        pix1[i + 2] *= (static_cast<float>(pix2[i + 1]) / 255.0f);
        pix1[i + 3] *= (static_cast<float>(pix2[i + 1]) / 255.0f);
    }
    // 解锁表面
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
    if (SDL_MUSTLOCK(mask)) SDL_UnlockSurface(mask);
    // 创建纹理
    SDL_Texture* img_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(mask);
    if (img_texture != NULL)
    {
        // 删除旧纹理
        auto iter = m_textures.find(_file_path);
        if (iter != m_textures.end()) SDL_DestroyTexture(iter->second);
        m_textures[_file_path] = img_texture;
    }
    else
    {
        SDL_Log("failed to create texture from %s with mask %s\n", _file_path.c_str(), _mask_path.c_str());
        return nullptr;
    }
    return m_textures[_file_path];
}

SDL_Texture* TextureRes::getTextureWithMask(const SDL_Color& color, const std::string& mask_path)
{
    SDL_Surface* mask = nullptr;
    mask = IMG_Load(mask_path.c_str());
    if (!ToRGBA8888(mask))
    {
        SDL_FreeSurface(mask);
        return nullptr;
    }
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, mask->w, mask->h, 32, SDL_PIXELFORMAT_RGBA8888);
    if (surface == nullptr)
    {
        SDL_Log("TextureRes::getTextureWithMask create surface failed\n");
        SDL_FreeSurface(mask);
        return nullptr;
    }
    // 锁定表面以获取访问权限
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
    if (SDL_MUSTLOCK(mask)) SDL_LockSurface(mask);
    // 实际数据
    Uint8* pix1 = (Uint8*)surface->pixels;
    Uint8* pix2 = (Uint8*)mask->pixels;
    // 逐位相乘
    size_t length = surface->pitch * surface->h;
    for (size_t i = 0;i < length;i += 4)
    {
        pix1[i] = color.a * (static_cast<float>(pix2[i + 1]) / 255.0f);
        pix1[i + 1] = color.b * (static_cast<float>(pix2[i + 1]) / 255.0f);
        pix1[i + 2] = color.g * (static_cast<float>(pix2[i + 1]) / 255.0f);
        pix1[i + 3] = color.r * (static_cast<float>(pix2[i + 1]) / 255.0f);
    }
    // 解锁表面
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
    if (SDL_MUSTLOCK(mask)) SDL_UnlockSurface(mask);
    // 创建纹理
    SDL_Texture* img_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(mask);
    if (img_texture != NULL)
    {
        // 删除旧纹理
        auto iter = m_textures.find(mask_path);
        if (iter != m_textures.end()) SDL_DestroyTexture(iter->second);
        m_textures[mask_path] = img_texture;
    }
    else
    {
        SDL_Log("failed to create texture from mask %s\n", mask_path.c_str());
        return nullptr;
    }
    return m_textures[mask_path];
}

SDL_Texture* TextureRes::getReanimTexture(const std::string& reanim_name)
{
    std::string real_image_name = toReal(reanim_name);
    std::string image_path = m_imageRootPath + "/" + real_image_name;
    if (m_textures.find(image_path) != m_textures.end())
    {
        return m_textures[image_path];
    }
    else
    {
        SDL_Texture* img_texture = IMG_LoadTexture(m_renderer, image_path.c_str());
        if (img_texture != NULL)
        {
            m_textures[image_path] = img_texture;
            return img_texture;
        }
        else
        {
            SDL_Log("failed to load: %s\n", image_path.c_str());
            return nullptr;
        }
    }
}

int TextureRes::FreeTexture(const std::string& _file_path)
{
    auto iter = m_textures.find(_file_path);
    if (iter != m_textures.end())
    {
        SDL_DestroyTexture(iter->second);
        m_textures.erase(iter);
    }
    return 0;
}

void TextureRes::CleanUp()
{
    for (auto tex : m_textures)
    {
        SDL_DestroyTexture(tex.second);
    }
}

TextureRes::~TextureRes()
{}

bool TextureRes::ToRGBA8888(SDL_Surface*& surface)
{
    if (surface == nullptr) return false;
    if (surface->format->format == SDL_PIXELFORMAT_RGBA8888) return true;
    SDL_Surface* pngSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    if (!pngSurface)
    {
        SDL_Log(SDL_GetError());
        return false;
    }
    else
    {
        SDL_FreeSurface(surface);
        surface = pngSurface;
        return true;
    }
}

void TextureRes::buildObjName()
{
    std::string first;
    for (const auto& second : ObjName)
    {
        if (second == "") continue;
        first = second;
        for (char& ch : first) ch = std::toupper(ch);
        m_objName[first] = second;
    }
}

std::string TextureRes::toReal(const std::string& reanim_name)
{
    // 如果是特殊的名称 应该在映射表中声明
    if (m_reanim2real.find(reanim_name) != m_reanim2real.end())
    {
        return m_reanim2real[reanim_name] + ".png";
    }
    else // 按照一般规律计算名称
    {
        // 找到从 IMAGE_REANIM_ 往后的第一个 _
        // 这之间的就是物体名称 该名称大小写混合 需要特别声明
        size_t obj_name_end_pos = reanim_name.find_first_of('_', 13);
        std::string obj_name = reanim_name.substr(13, obj_name_end_pos - 13);
        if (m_objName.find(obj_name) == m_objName.end())
        {
            SDL_Log("Resource ERROR: Unknown Reanim Image Object Name\n");
            return "";
        }
        obj_name = m_objName[obj_name];
        // 物体名称之后的字符全部小写
        for (char ch : reanim_name.substr(obj_name_end_pos))
        {
            obj_name += std::tolower(ch);
        }
        return obj_name + ".png";
    }
}