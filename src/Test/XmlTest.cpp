#include "Resource/tinyxml2.h"
#include <string>
#include <iostream>
#include "SDL.h"

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    std::string xml_path = "task/1-1-1.xml";
    tinyxml2::XMLDocument doc;
    int error = doc.LoadFile("task/1-1-1.xml");
    if (tinyxml2::XML_SUCCESS != error)
    {
        std::cout << "fail to load, error code: " << error << std::endl;
    }

    tinyxml2::XMLElement* item_ptr = doc.RootElement()->FirstChildElement();
    SDL_Log("%s\n", doc.RootElement()->Attribute("pp"));
    for (;item_ptr != nullptr;item_ptr = item_ptr->NextSiblingElement())
    {
        SDL_Log("%s\n", item_ptr->Name());
    }

    doc.SaveFile("output.xml");
}