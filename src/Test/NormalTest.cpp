#include <iostream>
#include <fstream>

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

int main()
{
    std::ifstream ifs("map/1-1.json");
    if (!ifs.is_open()) return -1;
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    assert(doc.IsObject());
    const rapidjson::Value& a = doc["time"];
    assert(a.IsObject());
    assert(doc["left"].IsNumber());
}