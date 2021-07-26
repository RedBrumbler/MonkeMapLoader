#pragma once

#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "modloader/shared/modloader.hpp"

#include "UnityEngine/Color.hpp"
#include "shared-constants.hpp"

/// finds a json property, or if it doesnt exist just doesnt assign a new value
#define GetJSONProperty(name, getter) \
    auto __##name ##_itr = val.FindMember(#name);\
    if (__##name ##_itr != val.MemberEnd()) {\
        name = __##name ##_itr->value.getter;\
    }

namespace MapLoader
{
    class MapConfig
    {
        public:
            MapConfig(rapidjson::Value& val)
            {
                imagePath = val["imagePath"].GetString();
                cubeMapImagePath = val["cubemapImagePath"].GetString();
                rapidjson::Value& color = val["mapColor"];

                float r = color["r"].GetFloat();
                float g = color["g"].GetFloat();
                float b = color["b"].GetFloat();
                mapColor = UnityEngine::Color(r, g, b);

                GetJSONProperty(gravity, GetFloat());
                GetJSONProperty(guid, GetString());
                GetJSONProperty(version, GetInt());

                auto modIDItr = val.FindMember("requiredModIDs");

                if (modIDItr != val.MemberEnd())
                {
                    assert(modIDItr->value.IsArray());

                    for (auto& id : modIDItr->value.GetArray())
                    {
                        requiredModIDs.push_back(id.GetString());
                    }
                }

                GetJSONProperty(slowJumpLimit, GetFloat());
                GetJSONProperty(fastJumpLimit, GetFloat());
                GetJSONProperty(slowJumpMultiplier, GetFloat());
                GetJSONProperty(fastJumpMultiplier, GetFloat());
                
                customData.SetObject();
                auto customData_itr = val.FindMember(customData);\
                if (customData_itr != val.MemberEnd()) {\
                    customData.CopyFrom(customData_itr->value, customData.GetAllocator());
                }
            }
            
            std::vector<std::string> GetMissingModIDs()
            {
                if (requiredModIDs.size() == 0) return std::vector<std::string>{};

                const std::unordered_map<std::string, const Mod> mods = Modloader::getMods();
                
                std::vector<std::string> missingIDs = {};

                for (auto& id : requiredModIDs)
                {
                    std::unordered_map<std::string, const Mod>::const_iterator it = mods.find(id);
                    if (it == mods.end()) missingIDs.push_back(id); 
                }

                return missingIDs;
            }

            std::string imagePath = "";
            std::string cubeMapImagePath = "";
            std::vector<std::string> requiredModIDs = {};
            UnityEngine::Color mapColor = {1.0f, 1.0f, 1.0f};
            float gravity = ::gravity;
            std::string guid = "";
            int version = 0;
            
            float slowJumpLimit = ::DefaultLimits.slowJumpLimit;
            float fastJumpLimit = ::DefaultLimits.fastJumpLimit;
            float slowJumpMultiplier = ::DefaultLimits.slowJumpMultiplier;
            float fastJumpMultiplier = ::DefaultLimits.fastJumpMultiplier;
            rapidjson::Document customData;
    };
}

#undef GetJSONProperty