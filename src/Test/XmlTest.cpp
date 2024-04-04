#include "Core/XmlLoader.h"

int main(int argc, char* args[])
{
    std::string xml_path = "resource/resource.xml";
    std::string xml_outpath = "./output.xml";
    XmlLoader loader;
    loader.Import(xml_path);
    std::cout << "root's attr prefix: " << loader.m_root->getAttr("prefix") << std::endl
        << "root's name: " <<  loader.m_root->getName() << std::endl;
    loader.m_root->getChild("ZOMBIE_HAND_ARM")->setAttr("test", "new attr");
    std::cout << "get path of ZOMBIE_HAND_ARM: " << loader.m_root->getChild("ZOMBIE_HAND_ARM")->getPath() << std::endl;
    loader.Export(xml_outpath);
}