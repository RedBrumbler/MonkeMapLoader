#include "Utils/RoomUtils.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Pun/ServerSettings.hpp"
#include "Photon/Realtime/LoadBalancingClient.hpp"
#include "Photon/Realtime/Room.hpp"

#include "GlobalNamespace/PhotonNetworkController.hpp"
#include "GlobalNamespace/GorillaComputer.hpp"

#include "ExitGames/Client/Photon/Hashtable.hpp"

#include "UnityEngine/PlayerPrefs.hpp"
#include "UnityEngine/SkinnedMeshRenderer.hpp"

#include "Behaviours/MapNetworkJoinTrigger.hpp"
#include "Behaviours/MonkeRoomManager.hpp"

#include "gorilla-utils/shared/Utils/Room.hpp"

extern Logger& getLogger();

using namespace Photon::Pun;
using namespace Photon::Realtime;
using namespace UnityEngine;

namespace MapLoader::RoomUtils
{
    void JoinModdedLobby(std::string map, bool casual)
    {
        std::string gameModeName = "infection_MOD_" + map;
        getLogger().info("Attempting to join a room with gamemode %s", gameModeName.c_str());
        GlobalNamespace::PhotonNetworkController* photonNetworkController = GlobalNamespace::PhotonNetworkController::_get_instance();

        bool InRoom = PhotonNetwork::get_InRoom();
        std::string gameMode = "";
        if (InRoom)
            gameMode = *GorillaUtils::Room::getRoomGameMode();

        photonNetworkController->pastFirstConnection = true;
        Il2CppString* gameType = il2cpp_utils::createcsstr(gameModeName);

        // if the player is in a private room
        if ((InRoom || photonNetworkController->attemptingToConnect) && *GorillaUtils::Room::getIsPrivate())
        {
            // if you can not find _MOD in the game mode, the player is in a non modded private
            std::string roomID = *GorillaUtils::Room::get_roomID();

            if (roomID.find("_MAP") == std::string::npos) 
            {
                roomID += "_MAP";
                GorillaUtils::Room::joinRoom(roomID);
            }
            return;
        }

        // casual queue support
        static Il2CppString* queueCS = il2cpp_utils::createcsstr(casual ? "CASUAL" : "DEFAULT", il2cpp_utils::StringType::Manual);
        GlobalNamespace::GorillaComputer::_get_instance()->currentQueue = queueCS;

        static Il2CppString* currentQueue = il2cpp_utils::createcsstr("currentQueue", il2cpp_utils::StringType::Manual);
        PlayerPrefs::SetString(currentQueue, queueCS);
        PlayerPrefs::Save();

        //MapNetworkJoinTrigger::get_instance()->gameModeName = gameType;

        std::string existingRoomName = MonkeRoomManager::get_instance()->GetLobbyIfExists(map);
        
        if (existingRoomName != "" && false)
        {
            photonNetworkController->currentGameType = gameType;
            GorillaUtils::Room::joinRoom(existingRoomName);
        }
        else
        {
            photonNetworkController->currentGameType = gameType;
            GorillaUtils::Room::joinPublic(gameModeName);
            //photonNetworkController->AttemptToJoinPublicRoom(MapNetworkJoinTrigger::get_instance());
        }
    }
}