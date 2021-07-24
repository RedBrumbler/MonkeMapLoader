#pragma once

#include "custom-types/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "Photon/Realtime/RoomInfo.hpp"

#include <string>
#include <vector>
#include <map>

struct RoomInfoData {
    std::string name = "";
    std::string gameMode = "";
    std::string region = "";
    int playerCount = 0;

    RoomInfoData() {};
    RoomInfoData(Photon::Realtime::RoomInfo* roomInfo);
    RoomInfoData(Photon::Realtime::RoomInfo* roomInfo, std::string region);
};

using MapToCount = std::map<std::string, int>;
using RegionToMap = std::map<std::string, MapToCount>;

DECLARE_CLASS_CODEGEN(MapLoader, MonkeRoomManager, Il2CppObject,
        DECLARE_CTOR(ctor);
        DECLARE_STATIC_METHOD(MonkeRoomManager*, get_instance);
        DECLARE_INSTANCE_METHOD(void, OnConnectedToMaster);        
        DECLARE_INSTANCE_METHOD(void, OnRoomListUpdate, List<Photon::Realtime::RoomInfo*>*);        
        DECLARE_STATIC_FIELD(int, updateCounter);

        public:
            static int PlayersOnMap(std::string mapName);
            static std::string GetLobbyIfExists(std::string mapName);
        private:
            static inline std::string forcedRegion = "";
            static inline std::vector<std::string>* checkedRegions = nullptr;
            static inline std::vector<RoomInfoData> roomListCache = {};
            static inline RegionToMap MapsInRegion = {};
)