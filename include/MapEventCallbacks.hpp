#pragma once

#include <functional>

namespace MapLoader::MapEventCallbacks
{
    typedef std::function<void(void)> MapEventCallback;

    void RegisterOnMapEnter(MapEventCallback callback);
    void RegisterOnMapLeave(MapEventCallback callback);
    void RegisterOnRoundEnd(MapEventCallback callback);

    void OnMapEnter();
    void OnMapLeave();
    void OnRoundEnd();
}