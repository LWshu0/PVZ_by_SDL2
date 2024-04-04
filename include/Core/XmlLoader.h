#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

class XmlNode;
typedef XmlNode* XmlNodePtr;

// 定义实体引用 用于内容的替换
static std::map<std::string, std::string> XmlEntityReference{
    {"&lt", "<"},
    {"&gt", ">"},
    {"&amp", "&"},
    {"&apos", "\'"},
    {"&quot", "\""}
};

class XmlNode {
protected:
    std::string m_nodeName;
    std::map<std::string, std::string> m_nodeAttrs;
    std::string m_nodeContent;
    std::vector<XmlNodePtr> m_childNodes;
    XmlNodePtr m_parentNode;
public:
    XmlNode();

    // 拷贝构造仅复制名称,属性,内容
    // 子节点置空, 父节点置 nullptr
    XmlNode(const XmlNode& other);

    // 获取节点名称
    inline std::string getName() { return m_nodeName; }
    // 根据属性名获取属性值
    std::string getAttr(const std::string& attr_name);
    // 获取节点内容
    inline std::string getContent() { return m_nodeContent; }
    // 获取所有子节点
    inline const std::vector<XmlNodePtr>& getChildren() { return m_childNodes; }
    // 获取指定名称的子节点
    std::vector<XmlNodePtr> getChildren(const std::string& child_node_name);
    // 获取第一个指定名称的子节点
    // 如果没有就返回 nullptr
    XmlNodePtr getChild(const std::string& child_node_name);
    // 获取父节点
    inline XmlNodePtr getParent() { return m_parentNode; }
    /**
     *@brief 获取从根节点开始的前缀, 用 / 号分隔. 例如: root/to/node 是节点的完整路径
     *       该函数返回 root/to
     * @return std::string 节点前缀
     */
    std::string getPrefix();
    // 返回节点的完整路径, 用 / 号分隔
    std::string getPath();
    // 返回根节点
    XmlNodePtr getRoot();
    // 返回根节点的名称
    std::string getRootName();

    // 克隆该节点以及其所有子节点(深拷贝)
    // 该节点的父节点置 nullptr
    XmlNodePtr clone();

    // 设置节点名称
    inline void setName(const std::string& node_name) { m_nodeName = node_name; }
    // 设置节点内容
    inline void setContent(const std::string& new_content) { m_nodeContent = new_content; }
    // 设置节点属性
    inline void setAttr(const std::string& attr_name, const std::string& attr_value) { m_nodeAttrs[attr_name] = attr_value; }
    // 删除节点属性
    int delAttr(const std::string& attr_name);

    // 添加子节点, 返回新添加的子节点的指针
    XmlNodePtr addChild(const XmlNode& child);
    XmlNodePtr addChild(XmlNodePtr child);
    int addChild(std::initializer_list<XmlNodePtr> new_nodes);

    // 删除子节点
    int removeAll(const std::string& node_name);
    int remove(int index);
    int clear();

    // 节点判断
    inline bool isRoot() { return nullptr == this->m_parentNode; }
    inline bool isLeaf() { return 0 == m_childNodes.size(); }

    int output(std::ofstream& fs, int space_num = 0);

    ~XmlNode();
protected:
    // 删除一个子节点
    std::vector<XmlNodePtr>::iterator remove(std::vector<XmlNodePtr>::const_iterator it);
};


class XmlLoader {
public:
    XmlNodePtr m_root;

protected:
    // 状态
    std::vector<std::string> node_stack;
public:
    XmlLoader();

    int Import(const std::filesystem::path& src_path);
    int Export(const std::filesystem::path& dst_path);

    ~XmlLoader();
protected:

    int ReadNode(std::string& str, size_t& pos, XmlNodePtr& node);

    // 检查是否有 xml head 如果有就跳过
    // 当前位置 pos 需在 '<' 处
    // 执行结束后 pos 在 '>' 后一个字符
    int CheckHead(std::string& str, size_t& pos);

    // 读取并设置节点名称
    // 当前位置 pos 需在 '<' 处, 执行后 pos 在名称结束后的第一个空白符处
    // 返回 0 代表成功, -1 代表错误
    int ReadName(std::string& str, size_t& pos, XmlNodePtr& node);

    // 读取并添加一个属性
    // 当前位置 pos 需要在属性开始的第一个字符处(使用 SkipSpace 跳过空白符)
    // 执行结束后 pos 在该属性结束后的第一个字符处(空白字符或'>')
    // 返回 0 代表成功, -1 代表错误
    int ReadAttr(std::string& str, size_t& pos, XmlNodePtr& node);

    // 读取标签之间的内容
    // 当前位置 pos 需要在 '>' 处
    // 执行结束后 pos 在 '<' 处
    // 返回 0 代表成功, -1 代表错误
    int ReadContent(std::string& str, size_t& pos, XmlNodePtr& node);

    // 读取并检查一个反标签是否闭合
    // 当前位置 pos 在 '<' 处
    // 执行结束后 pos 在 '>' 后的第一个字符处
    // 返回 0 代表成功, -1 代表错误
    int ReadCloseTag(std::string& str, size_t& pos, XmlNodePtr& node);

    // 查找第一个空白字符或 '>' 的位置
    // 用于找到一个节点名称或一个属性的结束位置
    size_t FirstSepPos(std::string& str, size_t pos);
    // 跳过空白字符, 将 pos 置于第一个非空白字符处
    // 返回 0 代表成功, -1 代表到达结尾(不修改pos)
    int SkipSpace(std::string& str, size_t& pos);
    int SkipComment(std::string& str, size_t& pos);

    bool isSpace(std::string& str, size_t pos);

    bool isHeadStart(std::string& str, size_t pos);
    bool isCommentStart(std::string& str, size_t pos);
    bool isOpenTagStart(std::string& str, size_t pos);
    bool isCloseTagStart(std::string& str, size_t pos);

    bool isHeadEnd(std::string& str, size_t pos);
    bool isCommentEnd(std::string& str, size_t pos);
    bool isTagEnd(std::string& str, size_t pos);
    // 单标签结束
    bool isSingleTagEnd(std::string& str, size_t pos);
};



#endif