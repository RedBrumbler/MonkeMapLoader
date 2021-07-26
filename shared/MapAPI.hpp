#pragma once

#include <optional>
#include <string>

namespace MapLoader::MapAPI
{
    std::optional<std::string> get_mapName();
    std::optional<std::string> get_mapFileName();
    std::optional<MapLoader::MapDescriptor> get_mapDescriptor();
}