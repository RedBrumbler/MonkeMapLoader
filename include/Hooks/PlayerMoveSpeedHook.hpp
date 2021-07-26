#pragma once
#include "Models/MapConfig.hpp"

namespace MapLoader
{
    class PlayerMoveSpeedHook
    {
        private:
            static bool needToSet;
            static MapConfig* config;
        public:
            static bool get_needToSet();
            static MapConfig* get_config();
            static void SetSpeed(MapConfig* MapConfig);
    };
}