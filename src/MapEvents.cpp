#include "MapEvents.hpp"

#include <vector>

#define EVENT(typename, getter) \
    typename __##typename; \
    typename& getter() { return __##typename; }

namespace MapLoader::MapEvents
{
    EVENT(OnMapEnterEvent, onMapEnterEvent);
    EVENT(OnMapLeaveEvent, onMapLeaveEvent);
    EVENT(OnRoundEndEvent, onRoundEndEvent);
    EVENT(OnMapLoaded, onMapLoaded);
}

#undef EVENT