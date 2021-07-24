#include "Behaviours/RoomList.hpp"
#include "Photon/Realtime/TypedLobby.hpp"
#include "Photon/Realtime/LobbyCallbacksContainer.hpp"
#include "Photon/Realtime/LoadBalancingClient.hpp"
#include "Photon/Realtime/ILobbyCallbacks.hpp"
#include "Photon/Pun/PhotonNetwork.hpp"

DEFINE_TYPE(MapLoader, RoomList);

using namespace UnityEngine;
using namespace Photon::Pun;
using namespace Photon::Realtime;

extern Logger& getLogger();

namespace MapLoader
{
    void RoomList::ctor()
    {
        instance = this;
        roomList = nullptr;

        LobbyCallbacksContainer* callbacksContainer = PhotonNetwork::_get_NetworkingClient()->LobbyCallbackTargets;
        if (callbacksContainer)
        {
            callbacksContainer->Add((ILobbyCallbacks*)this);
        }
        else getLogger().info("lobby callback container did not exist");

        getLogger().info("Getting custom room list");
        PhotonNetwork::GetCustomRoomList(TypedLobby::_get_Default(), System::String::_get_Empty());
    }

    RoomList* RoomList::get_instance()
    {
        return instance;
    }

    void RoomList::OnRoomListUpdate(List<RoomInfo*>* roomList)
    {
        getLogger().info("Room List callback!");
        this->roomList = roomList;
    }
}