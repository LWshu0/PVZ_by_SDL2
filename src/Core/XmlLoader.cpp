#include "Core/XmlLoader.h"

XmlNode::XmlNode() :
    m_nodeName(""),
    m_nodeContent(""),
    m_parentNode(nullptr)
{}

XmlNode::XmlNode(const XmlNode& other)
{
    this->m_nodeName = other.m_nodeName;
    for (auto& ele : other.m_nodeAttrs)
    {
        this->m_nodeAttrs[ele.first] = ele.second;
    }
    this->m_nodeContent = other.m_nodeContent;
    this->m_childNodes.clear();
    m_parentNode = nullptr;
}


std::string XmlNode::getAttr(const std::string& attr_name)
{
    auto iter = m_nodeAttrs.find(attr_name);
    if (m_nodeAttrs.end() == iter)
    {
        return "";
    }
    else
    {
        return iter->second;
    }
}

std::vector<XmlNodePtr> XmlNode::getChildren(const std::string& child_node_name)
{
    std::vector<XmlNodePtr> rtval;
    for (auto ptr : m_childNodes)
    {
        if (child_node_name == ptr->m_nodeName) rtval.push_back(ptr);
    }
    rtval.shrink_to_fit();
    return rtval;
}

XmlNodePtr XmlNode::getChild(const std::string& child_node_name)
{
    for (auto ptr : m_childNodes)
    {
        if (child_node_name == ptr->m_nodeName) return ptr;
    }
    return nullptr;
}

std::string XmlNode::getPrefix()
{
    std::string prefix = "";
    XmlNodePtr ptr = m_parentNode;
    if (nullptr != ptr)
    {
        prefix = ptr->m_nodeName;
        ptr = ptr->m_parentNode;
        while (nullptr != ptr)
        {
            prefix = ptr->m_nodeName + "/" + prefix;
            ptr = ptr->m_parentNode;
        }
    }
    return prefix;
}

std::string XmlNode::getPath()
{
    std::string path = m_nodeName;
    XmlNodePtr ptr = m_parentNode;
    while (nullptr != ptr)
    {
        path = ptr->m_nodeName + "/" + path;
        ptr = ptr->m_parentNode;
    }
    return path;
}

XmlNodePtr XmlNode::getRoot()
{
    XmlNodePtr rtval = this;
    while (!rtval->isRoot()) rtval = rtval->m_parentNode;
    return rtval;
}

std::string XmlNode::getRootName()
{
    return getRoot()->m_nodeName;
}

XmlNodePtr XmlNode::clone()
{
    XmlNodePtr rtval = new XmlNode();
    // std::cout << "new node" << std::endl;
    rtval->m_nodeName = this->m_nodeName;
    for (auto& iter : this->m_nodeAttrs)
    {
        rtval->m_nodeAttrs[iter.first] = iter.second;
    }
    rtval->m_nodeContent = this->m_nodeContent;
    for (auto ptr : this->m_childNodes)
    {
        rtval->m_childNodes.push_back(ptr->clone());
        rtval->m_childNodes.back()->m_parentNode = this;
    }
    rtval->m_parentNode = nullptr;
    return rtval;
}

int XmlNode::delAttr(const std::string& attr_name)
{
    auto it = m_nodeAttrs.find(attr_name);
    if (m_nodeAttrs.end() == it) return -1;
    else
    {
        m_nodeAttrs.erase(it);
        return 0;
    }
}

XmlNodePtr XmlNode::addChild(const XmlNode& child)
{
    XmlNodePtr new_node = new XmlNode(child);
    // std::cout << "new node" << std::endl;
    new_node->m_parentNode = this;
    m_childNodes.push_back(new_node);
    return new_node;
}

XmlNodePtr XmlNode::addChild(XmlNodePtr child)
{
    if (nullptr == child) return nullptr;
    XmlNodePtr new_node = new XmlNode(*child);
    new_node->m_parentNode = this;
    m_childNodes.push_back(new_node);
    return new_node;
}

int XmlNode::addChild(std::initializer_list<XmlNodePtr> new_nodes)
{
    int count = 0;
    for (auto& node : new_nodes)
    {
        if (node != nullptr)
        {
            count++;
            XmlNodePtr new_node = new XmlNode(*node);
            // std::cout << "new node" << std::endl;
            new_node->m_parentNode = this;
            m_childNodes.push_back(new_node);
        }
    }
    return count;
}

int XmlNode::removeAll(const std::string& m_nodeName)
{
    for (auto it = m_childNodes.begin();it != m_childNodes.end();)
    {
        if (m_nodeName == (*it)->m_nodeName)
        {
            it = remove(it);
        }
        else it++;
    }
    return 0;
}

int XmlNode::remove(int _index)
{
    if (_index < 0 || _index >= m_childNodes.size()) return -1;
    remove(m_childNodes.begin() + _index);
    return _index;
}

int XmlNode::clear()
{
    // 清空所有子节点
    for (auto child_iter = this->m_childNodes.begin();child_iter != this->m_childNodes.end();)
    {
        child_iter = this->remove(child_iter);
    }
    return 0;
}

int XmlNode::output(std::ofstream& fs, int space_num)
{
    std::string spaces(space_num, ' ');
    fs << spaces << "<" << m_nodeName;
    if (!m_nodeAttrs.empty())
    {
        for (auto& atr : m_nodeAttrs)
        {
            fs << " " << atr.first << "=\"" << atr.second << "\"";
        }
    }
    if (m_nodeContent == "" && m_childNodes.empty())
    {
        fs << "/>\n";
        return 0;
    }
    else fs << ">";
    if (m_nodeContent != "")
    {
        if (!m_childNodes.empty()) fs << "\n" << spaces << "    ";
        fs << m_nodeContent;
    }
    if (!m_childNodes.empty())
    {
        fs << "\n";
        for (auto& child : m_childNodes)
        {
            child->output(fs, space_num + 4);
        }
        fs << spaces;
    }
    fs << "</" << m_nodeName << ">\n";
    return 0;
}

XmlNode::~XmlNode()
{
    clear();
}

std::vector<XmlNodePtr>::iterator XmlNode::remove(std::vector<XmlNodePtr>::const_iterator it)
{
    // 首先清除所有子节点内存空间
    XmlNodePtr this_child_ptr = *it;
    for (auto child_iter = this_child_ptr->m_childNodes.begin(); child_iter != this_child_ptr->m_childNodes.end();)
    {
        child_iter = this_child_ptr->remove(child_iter);
    }
    delete this_child_ptr;
    // std::cout << "delete one node" << std::endl;
    return m_childNodes.erase(it);
}


XmlLoader::XmlLoader()
{
    m_root = new XmlNode();
}

int XmlLoader::Import(const std::filesystem::path& src_path)
{
    std::fstream file(src_path);
    if (!file.is_open()) return -1;
    std::string file_content = "";
    std::string line;
    char temp_ch;
    while (std::getline(file, line)) file_content += line;
    file.close();
    size_t now_pos = 0;
    XmlNodePtr now_node = m_root;
    // 检查第一行
    if (-1 == CheckHead(file_content, now_pos))
    {
        std::cout << "没有发现 xml 声明" << std::endl;
    }
    // 读取 xml 文档内容
    while (now_pos < file_content.length())
    {
        ReadNode(file_content, now_pos, now_node);
    }
    return node_stack.empty() ? 0 : -1;
}

int XmlLoader::Export(const std::filesystem::path& dst_path)
{
    std::ofstream file(dst_path);
    if (!file.is_open())
    {
        std::cout << "创建文件失败" << std::endl;
        return -1;
    }
    m_root->output(file);
    file.close();
    return 0;
}

XmlLoader::~XmlLoader()
{
    // 清空所有树
    m_root->clear();
    delete m_root;
}

int XmlLoader::ReadNode(std::string& str, size_t& pos, XmlNodePtr& node)
{
    // 忽略前缀空格
    if (-1 == SkipSpace(str, pos)) return -1;
    // 检查开标签
    if (!isOpenTagStart(str, pos)) return -1;
    // 读取节点名称
    if (-1 == ReadName(str, pos, node)) return -1;
    // 读取属性
    while (1)
    {
        // 跳过空白符
        if (-1 == SkipSpace(str, pos)) return -1;
        // 如果遇到结束符 结束读取属性
        if (isTagEnd(str, pos)) break;
        // 读取一个属性并添加
        if (-1 == ReadAttr(str, pos, node)) return -1;
        // 跳过空白符
        if (-1 == SkipSpace(str, pos)) return -1;
        // 无内容节点特殊处理 读取完毕 标签出栈 回退父节点 直接返回
        if (isSingleTagEnd(str, pos))
        {
            pos += 2;
            if (pos > str.length()) return -1;
            node_stack.pop_back();
            node = node->getParent();
            return 0;
        }
    }
    // 可能是子节点或内容 分情况

    size_t temp_pos = pos + 1;
    // 判断是子节点还是内容
    if (temp_pos >= str.length() || -1 == SkipSpace(str, temp_pos)) return -1;    // 错误
    if (isOpenTagStart(str, temp_pos))
    {
        do
        {
            // 读取子节点 递归
            node = node->addChild(XmlNode());
            if (-1 == ReadNode(str, temp_pos, node)) return -1;
            // 跳过空白符 到下一个标签起始点
            if (-1 == SkipSpace(str, temp_pos)) return -1;
        } while (!isCloseTagStart(str, temp_pos));
        // 如果不是开标签 就是到达了闭合标签
        pos = temp_pos;
    }
    else
    {
        // 读取内容
        if (-1 == ReadContent(str, pos, node)) return -1;
    }

    // 跳过空白符
    if (-1 == SkipSpace(str, pos)) return -1;
    // 读取闭合标签
    if (-1 == ReadCloseTag(str, pos, node)) return -1;

    return 0;
}

int XmlLoader::CheckHead(std::string& str, size_t& pos)
{
    if (!isHeadStart(str, pos))
    {
        // std::cout << "xml 声明错误, 起始标识错误" << std::endl;
        return -1;
    }
    pos = str.find_first_of('>', pos);
    if (std::string::npos == pos || !isHeadEnd(str, pos - 1))
    {
        std::cout << "xml 声明错误, 没有结束标识" << std::endl;
        return -1;
    }
    pos += 1;
    if (pos >= str.length())
    {
        std::cout << "xml 内容为空" << std::endl;
        return -1;
    }
    return 0;
}

int XmlLoader::ReadName(std::string& str, size_t& pos, XmlNodePtr& node)
{
    // 检查 '<' 后第一个字符是否是空白符
    if (pos + 1 >= str.length())
    {
        std::cout << "error: 没有找到名称起始点" << std::endl;
        return -1;
    }
    pos += 1;
    if (isSpace(str, pos))
    {
        std::cout << "error: 名称前不能有空白符" << std::endl;
        return -1;
    }
    size_t name_end_pos = FirstSepPos(str, pos);
    if (std::string::npos == name_end_pos)
    {
        std::cout << "不完整的节点起始标签, 名称没有结束点" << std::endl;
        return -1;
    }
    std::string m_nodeName = str.substr(pos, name_end_pos - pos);
    // 实体引用替换
    // ...
    // 压栈记录
    node_stack.push_back(m_nodeName);

    node->setName(m_nodeName);
    pos = name_end_pos;
    return 0;
}

int XmlLoader::ReadAttr(std::string& str, size_t& pos, XmlNodePtr& node)
{
    if (std::string::npos == pos)
    {
        std::cout << "error: 属性名称未找到" << std::endl;
        return -1;
    }
    size_t attr_name_end_pos = str.find_first_of(" =\n\t\r", pos);
    if (std::string::npos == attr_name_end_pos)
    {
        std::cout << "error: 属性名称没有结束" << std::endl;
        return -1;
    }
    std::string attr_name = str.substr(pos, attr_name_end_pos - pos);
    pos = attr_name_end_pos;    // 属性名后第一个空白字符或等号位置
    if (-1 == SkipSpace(str, pos) || str[pos] != '=')
    {
        std::cout << "error: 属性名称没有赋值" << std::endl;
        return -1;
    }
    pos += 1;
    if (-1 == SkipSpace(str, pos))
    {
        std::cout << "error: 属性值没有找到" << std::endl;
        return -1;
    }
    if ('\"' != str[pos] && '\'' != str[pos])
    {
        std::cout << "error: 属性值必须添加左引号" << std::endl;
        return -1;
    }
    size_t attr_value_start_pos = pos + 1;
    if (attr_value_start_pos >= str.length())
    {
        std::cout << "error: 属性值没有找到" << std::endl;
        return -1;
    }
    size_t attr_value_end_pos = str.find_first_of(str[pos], attr_value_start_pos);   // 找闭合引号
    if (std::string::npos == attr_value_end_pos)
    {
        std::cout << "error: 属性值必须添加右引号" << std::endl;
        return -1;
    }
    std::string attr_value = str.substr(attr_value_start_pos, attr_value_end_pos - attr_value_start_pos);
    node->setAttr(attr_name, attr_value);
    // 属性后面至少还有一个字符 '>' 或 空白字符用于分隔下一个属性
    pos = attr_value_end_pos + 1;
    if (pos < str.length()) return 0;
    else return -1;
}

int XmlLoader::ReadContent(std::string& str, size_t& pos, XmlNodePtr& node)
{
    if (pos >= str.length() - 1 || str[pos] != '>')
    {
        std::cout << "没有找到节点内容起始位置" << std::endl;
        return -1;
    }
    size_t content_start_pos = pos + 1;
    // 内容结束位置
    size_t end_content_pos = str.find_first_of("<", pos);
    if (end_content_pos == std::string::npos)
    {
        std::cout << "没有找到节点内容结束位置" << std::endl;
        return -1;
    }
    std::string content = str.substr(content_start_pos, end_content_pos - content_start_pos);
    // todo: 实体引用替换
    // ...
    node->setContent(content);
    // 将 pos 放置到右标签起始 '<' 处
    pos = end_content_pos;
    return 0;
}

int XmlLoader::ReadCloseTag(std::string& str, size_t& pos, XmlNodePtr& node)
{
    if (!isCloseTagStart(str, pos))
    {
        std::cout << "不是节点的闭合标签" << std::endl;
        return -1;
    }
    pos += 2;   // 标签名称第一个字符
    if (pos >= str.length() || isSpace(str, pos))
    {
        std::cout << "闭合标签格式错误" << std::endl;
        return -1;
    }
    size_t close_tag_end_pos = FirstSepPos(str, pos);
    if (std::string::npos == close_tag_end_pos)
    {
        std::cout << "闭合标签格式没有 \'>\'" << std::endl;
        return -1;
    }
    std::string close_tag = str.substr(pos, close_tag_end_pos - pos);
    // 将 pos 移动到指定位置
    pos = str.find_first_of('>', close_tag_end_pos) + 1;

    if (close_tag == node_stack.back()) // 闭合标签匹配
    {
        node_stack.pop_back();
        node = node->getParent();  // 回到父节点
        return 0;
    }
    else
    {
        std::cout << "标签不匹配, 期望: \"" << node_stack.back() << "\", 实际: \"" << close_tag << "\"" << std::endl;
        return -1;
    }
}

size_t XmlLoader::FirstSepPos(std::string& str, size_t pos)
{
    return str.find_first_of(" >\n\t\r", pos);
}

int XmlLoader::SkipSpace(std::string& str, size_t& pos)
{
    size_t new_pos = str.find_first_not_of(" \n\t\r", pos);
    if (std::string::npos == new_pos) return -1;
    // 忽略注释以及注释后的空行
    if (isCommentStart(str, new_pos))
    {
        if (-1 == SkipComment(str, new_pos)) return -1;
        new_pos = str.find_first_not_of(" \n\t\r", new_pos);
        if (std::string::npos == new_pos) return -1;
    }
    pos = new_pos;
    return 0;
}

int XmlLoader::SkipComment(std::string& str, size_t& pos)
{
    size_t new_pos = str.find_first_of(">", pos);
    if (std::string::npos == new_pos)
    {
        std::cout << "error: can't find comment end tag \'>\'" << std::endl;
        return -1;
    }
    pos = new_pos - 2;   // 找到 "-->" 的第一个字符位置
    if (!isCommentEnd(str, pos))
    {
        std::cout << "error: comment tag not close" << std::endl;
        return -1;
    }
    pos = new_pos + 1;
    if (pos >= str.length())
    {
        // 文档读取结束
        std::cout << "waring: string out of range when skip comment" << std::endl;
        pos = std::string::npos;
        return -1;
    }
    return 0;
}

bool XmlLoader::isSpace(std::string& str, size_t pos)
{
    return str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\t' || str[pos] == '\r';
}

bool XmlLoader::isHeadStart(std::string& str, size_t pos)
{
    return "<?xml" == str.substr(pos, 5);
}

bool XmlLoader::isCommentStart(std::string& str, size_t pos)
{
    return "<!--" == str.substr(pos, 4);
}

bool XmlLoader::isOpenTagStart(std::string& str, size_t pos)
{
    return '<' == str[pos] && !isCommentStart(str, pos) && !isHeadStart(str, pos) && !isCloseTagStart(str, pos);
}

bool XmlLoader::isCloseTagStart(std::string& str, size_t pos)
{
    return "</" == str.substr(pos, 2);
}

bool XmlLoader::isHeadEnd(std::string& str, size_t pos)
{
    if (-1 == SkipSpace(str, pos)) return false;
    return "?>" == str.substr(pos, 2);
}

bool XmlLoader::isCommentEnd(std::string& str, size_t pos)
{
    if (-1 == SkipSpace(str, pos)) return false;
    return "-->" == str.substr(pos, 3);
}

bool XmlLoader::isTagEnd(std::string& str, size_t pos)
{
    if (std::string::npos == pos) return false;
    return '>' == str[pos];
}

bool XmlLoader::isSingleTagEnd(std::string& str, size_t pos)
{
    if (-1 == SkipSpace(str, pos)) return false;
    return "/>" == str.substr(pos, 2);
}