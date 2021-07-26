#pragma once

#include <functional>
#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace UnityEngine {
    class GameObject;
}

namespace MapLoader::MapEvents
{
    typedef UnorderedEventCallback<> OnMapEnterEvent;
    typedef UnorderedEventCallback<> OnMapLeaveEvent;
    typedef UnorderedEventCallback<> OnRoundEndEvent;
    
    typedef UnorderedEventCallback<const rapidjson::Document&, UnityEngine::GameObject*> OnMapLoaded;
    
    /// @brief event that gets ran when entering a map
    OnMapEnterEvent& onMapEnterEvent();

    /// @brief event that gets ran when leaving a map
    OnMapLeaveEvent& onMapLeaveEvent();

    /// @brief event that gets ran when the round ends (any tag in tag, everyone tagged in infection)
    OnRoundEndEvent& onRoundEndEvent();

    /// @brief event that gets ran when a new map is loaded
    OnMapLoaded& onMapLoaded();
}