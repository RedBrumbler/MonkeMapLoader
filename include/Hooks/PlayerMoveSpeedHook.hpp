#pragma once
#include "Models/MapConfig.hpp"

namespace MapLoader
{
    class PlayerMoveSpeedHook
    {
        private:
            static bool needToSet;
        public:
            static float slowJumpLimit;
            static float fastJumpLimit;
            static float slowJumpMultiplier;
            static float fastJumpMultiplier;
            static bool get_needToSet();
            static void SetSpeed(MapConfig& MapConfig);
    };
}