#ifndef TEXTURERES_H
#define TEXTURERES_H

#include <string>
#include <vector>
#include <map>

#include "SDL.h"
#include "SDL_image.h"
#include "Resource/tinyxml2.h"

class TextureRes {
protected:
    // 图像路径到纹理的映射
    std::map<std::string, SDL_Texture*> m_textures;
    // Reanim 图像名称到真实图像名称的映射
    std::map<std::string, std::string> m_reanim2real;
    // 图片的根路径
    std::string m_imageRootPath;
    // reanim 物体名(大写)到真实物体名(大小写混合)的映射
    std::map<std::string, std::string> m_objName;
public:
    TextureRes() = default;
    // 给定渲染器
    // 资源映射文件路径 
    // 图片资源文件夹路径 如图片路径为 res/image/a.png, 需要传入 res/image
    TextureRes(
        const std::string& xml_path,
        const std::string& image_path
    );

    void initilize(
        const std::string& xml_path,
        const std::string& image_path
    );

    // 根据图片的路径(限定在 png 格式)获取图像纹理
    SDL_Texture* getTextureFrom(const std::string& file_path);

    // 将原图片按比特乘以蒙版图片
    // 要求两者大小必须相同
    SDL_Texture* getTextureWithMask(const std::string& file_path, const std::string& mask_path);

    // 将 mask 赋值颜色后返回纹理
    // 纹理使用 mask 的路径访问
    SDL_Texture* getTextureWithMask(const SDL_Color& color, const std::string& mask_path);

    // 根据 reanim 图像名称获取纹理
    SDL_Texture* getReanimTexture(const std::string& reanim_name);

    // 释放指定的纹理
    int FreeTexture(const std::string& file_path);

    // 释放纹理资源 必需在 SDL_Quit 之前调用
    void clean();

    ~TextureRes();
protected:
    bool ToRGBA8888(SDL_Surface*& surface);

    void buildObjName();

    std::string toReal(const std::string& reanim_name);
};

static std::vector<std::string> ObjName = {
    // 植物
    "PeaShooter",       "SunFlower",        "CherryBomb",       "Wallnut",
    "PotatoMine",       "SnowPea",          "Chomper",
    "PuffShroom",       "SunShroom",        "FumeShroom",       "Gravebuster",
    "HypnoShroom",      "ScaredyShroom",    "IceShroom",        "DoomShroom",
    "LilyPad",          "Squash",           "ThreePeater",      "Tanglekelp",
    "Jalapeno",         "Caltrop",          "Torchwood",        "Tallnut",
    "SeaShroom",        "Plantern",         "Cactus",           "Blover",
    "SplitPea",         "Starfruit",        "Pumpkin",          "Magnetshroom",
    "Cabbagepult",      "Pot",              "Cornpult",         "Coffeebean",
    "Garlic",           "Umbrellaleaf",     "Marigold",         "Melonpult",
    "GatlingPea",       "TwinSunflower",    "GloomShroom",      "Cattail",
    "WinterMelon",      "GoldMagnet",       "SpikeRock",        "CobCannon",
    "Imitater",

    "ExplodeNut",       "GiantWallNut", // 此行无素材
    // 僵尸
    "Zombie"
};

#endif