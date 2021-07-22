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

extern Logger& getLogger();

using namespace Photon::Pun;
using namespace Photon::Realtime;
using namespace UnityEngine;

namespace GorillaUI::BaseGameInterface
{
    void JoinRoom(std::string roomID);

    namespace Room
    {
        std::string get_roomID();
    }
}

namespace MapLoader::RoomUtils
{
    void JoinModdedLobby(std::string map)
    {
        std::string gameModeName = "infection_MOD_" + map;
        getLogger().info("Attempting to join a room with gamemode %s", gameModeName.c_str());
        GlobalNamespace::PhotonNetworkController* photonNetworkController = GlobalNamespace::PhotonNetworkController::_get_instance();

        bool InRoom = PhotonNetwork::get_InRoom();

        Room* currentRoom = PhotonNetwork::get_CurrentRoom();
        std::string gameMode = "";
        if (currentRoom)
        {
            ExitGames::Client::Photon::Hashtable* customProperties = currentRoom->get_CustomProperties();
            static Il2CppString* gameModeLabel = il2cpp_utils::createcsstr("gameMode", il2cpp_utils::StringType::Manual);
            Il2CppString* gameModeCS = (Il2CppString*)customProperties->get_Item((Il2CppObject*)gameModeLabel);
            gameMode = gameModeCS ? to_utf8(csstrtostr(gameModeCS)) : "";
        }

        photonNetworkController->pastFirstConnection = true;
        Il2CppString* gameType = il2cpp_utils::createcsstr(gameModeName);

        // if the player is in a private room
        if ((InRoom || photonNetworkController->attemptingToConnect) && gameMode.find("private") != std::string::npos)
        {
            // if you can not find _MOD in the game mode, the player is in a non modded private
            std::string roomID = GorillaUI::BaseGameInterface::Room::get_roomID();

            if (roomID.find("_MAP") == std::string::npos) 
            {
                roomID += "_MAP";
                GorillaUI::BaseGameInterface::JoinRoom(roomID);
            }

            return;
        }


        static Il2CppString* queueCS = il2cpp_utils::createcsstr("DEFAULT", il2cpp_utils::StringType::Manual);
        GlobalNamespace::GorillaComputer::_get_instance()->currentQueue = queueCS;

        static Il2CppString* currentQueue = il2cpp_utils::createcsstr("currentQueue", il2cpp_utils::StringType::Manual);
        PlayerPrefs::SetString(currentQueue, queueCS);
        PlayerPrefs::Save();

        MapNetworkJoinTrigger::get_instance()->gameModeName = gameType;

        std::string lobbyName = MonkeRoomManager::get_instance()->GetLobbyIfExists(map);
        
        if (lobbyName != "" && false)
        {
            photonNetworkController->currentGameType = gameType;
            GorillaUI::BaseGameInterface::JoinRoom(lobbyName);
        }
        else
        {
            photonNetworkController->currentGameType = gameType;
            photonNetworkController->AttemptToJoinPublicRoom(MapNetworkJoinTrigger::get_instance());
        }
        /*
        if (InRoom && gameMode != gameModeName)
        {
            photonNetworkController->currentGameType = gameType;
            photonNetworkController->attemptingToConnect = true;

            Array<SkinnedMeshRenderer*>* offlineVRRig = photonNetworkController->offlineVRRig;

            for (int i = 0; i < offlineVRRig->Length(); i++)
            {
                offlineVRRig->values[i]->set_enabled(true);
            }

            photonNetworkController->AttemptDisconnect();
            getLogger().info("Disconnected from room");
            return;
        }

        bool attemptingToConnect = photonNetworkController->attemptingToConnect;
        if (!InRoom && !attemptingToConnect)
        {
            MapNetworkJoinTrigger::get_instance()->gameModeName = gameType;
            photonNetworkController->AttemptToJoinPublicRoom(MapNetworkJoinTrigger::get_instance());
            getLogger().info("Attempting room connect");
        }
        */
    }
}