#include "MapEventCallbacks.hpp"

#include <vector>

namespace MapLoader::MapEventCallbacks
{
    std::vector<MapEventCallback> onMapEnter;
    std::vector<MapEventCallback> onMapLeave;
    std::vector<MapEventCallback> onRoundEnd;

    void RegisterOnMapEnter(MapEventCallback callback)
    {
        onMapEnter.push_back(callback);
    }

    void RegisterOnMapLeave(MapEventCallback callback)
    {
        onMapLeave.push_back(callback);
    }

    void RegisterOnRoundEnd(MapEventCallback callback)
    {
        onRoundEnd.push_back(callback);
    }

    void OnMapEnter()
    {
        for (auto& event : onMapEnter)
        {
            event();
        }
    }

    void OnMapLeave()
    {
        for (auto& event : onMapLeave)
        {
            event();
        }
    }

    void OnRoundEnd()
    {
        for (auto& event : onRoundEnd)
        {
            event();
        }
    }
}