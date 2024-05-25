#include "Core/Dictionary.h"
#include "Core/GlobalVars.h"

Dictionary::Dictionary(
    const std::string& imageFilePath,
    const std::string& wordFilePath,
    const SDL_Color& color
)
{
    m_wordTable = GlobalVars::getInstance().textureRes.getTextureWithMask(color, imageFilePath);
    m_charList.clear();
    // 读取文件
    std::fstream file(wordFilePath);
    if (file.is_open())
    {
        std::string content;
        std::string line;
        while (getline(file, line)) { content += line + "\n"; }
        size_t cur = 0;
        size_t left, right;
        do
        {
            left = content.find_first_of('D', cur);
            if (std::string::npos == left)
            {
                break;
            }
            if (content.substr(left, 6) != "Define")
            {
                cur = left + 6;
                continue;
            }
            left = left + 7;
            right = content.find_first_of("\n\r", left);
            std::string tag = content.substr(left, right - left);
            cur = right + 1;
            if (tag == "CharList0") cur = CharList0(content, cur);
            else if (tag == "WidthList0") cur = WidthList0(content, cur);
            else if (tag == "RectList0") cur = RectList0(content, cur);
            else if (tag == "OffsetList0") cur = OffsetList0(content, cur);
            else
            {
                // SDL_Log("error: in Dictionary::Dictionary, wrong tag\n");
                break;
            }
            cur = right;
        } while (cur < content.length());
    }
    else
    {
        SDL_Log("can't load %d\n", wordFilePath.c_str());
    }
    // for (int i = 0;i < m_charList.size();i++)
    // {
    //     SDL_Log("%s, width: %d, rect: (%d, %d, %d, %d), offset: (%d, %d)\n", m_charList[i].m_charValue.c_str(), m_charList[i].m_charWidth, m_charList[i].m_charRect.x, m_charList[i].m_charRect.y, m_charList[i].m_charRect.w, m_charList[i].m_charRect.h, m_charList[i].m_charOffset.x, m_charList[i].m_charOffset.y);
    // }
}

int Dictionary::render(const std::string& str, int x, int y, float scale)
{
    SDL_Rect src{ x, y, 0, 0 };
    SDL_Rect dst{ x, y, 0, 0 };
    for (int i = 0;i < str.length();)
    {
        int char_width = u8charbyte(str[i]);
        int idx = findChar(str.substr(i, char_width));
        src = m_charList[idx].m_charRect;
        dst.w = src.w * scale;
        dst.h = src.h * scale;
        SDL_RenderCopy(GlobalVars::getInstance().renderer, m_wordTable, &src, &dst);
        dst.x += m_charList[idx].m_charWidth * scale;
        // next u8 char
        i += char_width;
    }
    return 0;
}

Dictionary::~Dictionary()
{}

size_t Dictionary::CharList0(const std::string& input, size_t cur)
{
    int charIdx = 0;
    size_t left, right;
    while (1)
    {
        left = input.find_first_of('\'', cur);
        if (input[left + 1] == '\\')
        {
            right = input.find_first_of('\'', left + 3);
        }
        else right = input.find_first_of('\'', left + 2);

        if (m_charList.size() <= charIdx)
        {
            m_charList.push_back(CharListNode());
        }
        m_charList[charIdx].m_charValue = input.substr(left + 1, right - left - 1);
        charIdx += 1;
        cur = right + 1;
        if (input[right + 2] == ')' && input[right + 3] == ';') break;
    }
    return cur;
}
size_t Dictionary::WidthList0(const std::string& input, size_t cur)
{
    int charIdx = 0;
    size_t left, right;
    while (1)
    {
        if (m_charList.size() <= charIdx)
        {
            break;
        }
        left = find_first_digit(input, cur);
        right = find_first_not_digit(input, left);
        m_charList[charIdx].m_charWidth = std::stoi(input.substr(left, right - left));
        charIdx += 1;
        cur = right + 1;
    }
    return cur;
}
size_t Dictionary::RectList0(const std::string& input, size_t cur)
{
    int charIdx = 0;
    size_t left, right;
    while (1)
    {
        if (m_charList.size() <= charIdx)
        {
            break;
        }

        left = find_first_digit(input, cur);
        right = find_first_not_digit(input, left);
        m_charList[charIdx].m_charRect.x = std::stoi(input.substr(left, right - left));
        cur = right + 1;

        left = find_first_digit(input, cur);
        right = find_first_not_digit(input, left);
        m_charList[charIdx].m_charRect.y = std::stoi(input.substr(left, right - left));
        cur = right + 1;

        left = find_first_digit(input, cur);
        right = find_first_not_digit(input, left);
        m_charList[charIdx].m_charRect.w = std::stoi(input.substr(left, right - left));
        cur = right + 1;

        left = find_first_digit(input, cur);
        right = find_first_not_digit(input, left);
        m_charList[charIdx].m_charRect.h = std::stoi(input.substr(left, right - left));
        cur = right + 1;

        charIdx += 1;
    }
    return cur;
}
size_t Dictionary::OffsetList0(const std::string& input, size_t cur)
{
    int charIdx = 0;
    size_t left, right;
    while (1)
    {
        if (m_charList.size() <= charIdx)
        {
            break;
        }

        left = find_first_digit(input, cur);
        right = find_first_not_digit(input, left);
        m_charList[charIdx].m_charOffset.x = -std::stoi(input.substr(left, right - left));
        cur = right + 1;

        left = find_first_digit(input, cur);
        right = find_first_not_digit(input, left);
        m_charList[charIdx].m_charOffset.y = -std::stoi(input.substr(left, right - left));
        cur = right + 1;

        charIdx += 1;
    }
    return cur;
}

size_t Dictionary::find_first_digit(const std::string& input, size_t cur)
{
    while (cur < input.size())
    {
        if (input[cur] >= '0' && input[cur] <= '9')
        {
            return cur;
        }
        cur++;
    }
    return std::string::npos;
}

size_t Dictionary::find_first_not_digit(const std::string& input, size_t cur)
{
    while (cur < input.size())
    {
        if (input[cur] < '0' || input[cur] > '9')
        {
            return cur;
        }
        cur++;
    }
    return std::string::npos;
}

int Dictionary::findChar(const std::string& chara)
{
    for (int i = 0;i < m_charList.size();i++)
    {
        if (chara == m_charList[i].m_charValue)
        {
            return i;
        }
    }
    return -1;
}

size_t Dictionary::u8charbyte(char ch)
{
    if ((ch & 0x80) == 0x00)
    { // ASCII 1字节字符
        return 1;
    }
    else if ((ch & 0xe0) == 0xc0)
    { // 2字节字符
        return 2;
    }
    else if ((ch & 0xf0) == 0xe0)
    { // 3字节字符
        return 3;
    }
    else if ((ch & 0xf8) == 0xf0)
    { // 4字节字符
        return 4;
    }
    else return 0;
}