#pragma once

#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "modloader/shared/modloader.hpp"

#include "UnityEngine/Color.hpp"
#include "shared-constants.hpp"

namespace MapLoader
{
    class MapConfig
    {
        public:
            MapConfig(rapidjson::Value& val);
            std::vector<std::string> GetMissingModIDs();

            std::string imagePath;
            std::string cubeMapImagePath;
            std::vector<std::string> requiredModIDs;
            UnityEngine::Color mapColor;
            float gravity;
            std::string gameMode;
            std::string guid;
            int version;
            
            float slowJumpLimit;
            float fastJumpLimit;
            float slowJumpMultiplier;
            float fastJumpMultiplier;
            rapidjson::Document customData;
    };
}