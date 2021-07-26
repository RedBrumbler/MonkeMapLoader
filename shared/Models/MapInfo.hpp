#pragma once

#include <string>
#include "Models/MapPackageInfo.hpp"
#include "UnityEngine/Texture2D.hpp"
namespace MapLoader
{
    class MapInfo
    {
        public:
            bool operator<(const MapInfo& second) const;
            MapInfo(std::string filePath) : filePath(filePath) {};
            std::string get_mapString();
            void Load();
            UnityEngine::Texture2D* LoadCubeMap();
            std::string filePath;
            MapPackageInfo* packageInfo;
    };
}