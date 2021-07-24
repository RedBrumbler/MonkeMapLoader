#pragma once

#include "custom-types/shared/macros.hpp"

#include "Photon/Pun/MonoBehaviourPunCallbacks.hpp"
#include "Photon/Realtime/RoomInfo.hpp"

DECLARE_CLASS_CODEGEN(MapLoader, RoomList, Photon::Pun::MonoBehaviourPunCallbacks,
    DECLARE_OVERRIDE_METHOD(void, OnRoomListUpdate, il2cpp_utils::FindMethodUnsafe("Photon.Pun", "MonoBehaviourPunCallbacks", "OnRoomListUpdate", 1), List<Photon::Realtime::RoomInfo*>* roomList);
    DECLARE_STATIC_METHOD(RoomList*, get_instance);
    DECLARE_CTOR(ctor);
    DECLARE_INSTANCE_FIELD(List<Photon::Realtime::RoomInfo*>*, roomList);
    
    private:
        static inline RoomList* instance = nullptr;
)