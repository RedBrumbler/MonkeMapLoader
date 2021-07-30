#pragma once

#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace MapLoader
{
    class Descriptor
    {
        public:
            Descriptor(rapidjson::Value& val);
            
            std::string author;
            std::string mapName;
            std::string description;
            std::string androidRequiredVersion = "1.0.0";
    };
}