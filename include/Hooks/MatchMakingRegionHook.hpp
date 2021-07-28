#pragma once

#include "GlobalNamespace/PhotonNetworkController.hpp"
#include "GlobalNamespace/PhotonNetworkController_ConnectionEvent.hpp"
#include <vector>

namespace MapLoader
{
    class MatchMakingRegionHook
    {
        public:
            static inline bool firstRegion = true;
            static inline std::vector<int> usedRegions = {};
            static bool ProcessJoiningPublicRoomState_Prefix(GlobalNamespace::PhotonNetworkController* self, GlobalNamespace::PhotonNetworkController::ConnectionEvent connectionEvent);
    };
}