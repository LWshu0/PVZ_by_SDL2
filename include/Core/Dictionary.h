#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <fstream>
#include <map>
#include <vector>

#include "SDL.h"

struct CharListNode {
    std::string m_charValue = "";
    int m_charWidth = 0;
    SDL_Rect m_charRect = SDL_Rect{ 0,0,0,0 };
    SDL_Point m_charOffset = SDL_Point{ 0,0 };
};

class Dictionary {
protected:

    SDL_Texture* m_wordTable;
    std::vector<CharListNode> m_charList;

public:
    Dictionary(
        const std::string& imageFilePath,
        const std::string& wordFilePath,
        const SDL_Color& color = SDL_Color{ 255,255,255,255 }
    );

    int render(const std::string& str, int x, int y, float scale = 1.0f);

    ~Dictionary();
protected:
    size_t CharList0(const std::string& input, size_t cur);
    size_t WidthList0(const std::string& input, size_t cur);
    size_t RectList0(const std::string& input, size_t cur);
    size_t OffsetList0(const std::string& input, size_t cur);

    size_t find_first_digit(const std::string& input, size_t cur = 0);
    size_t find_first_not_digit(const std::string& input, size_t cur = 0);

    int findChar(const std::string& chara);
    size_t u8charbyte(char ch);
};

#endif