#include "Behaviours/MonkeRoomManager.hpp"

DEFINE_TYPE(MapLoader, MonkeRoomManager);

#include <vector>

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Pun/ServerSettings.hpp"

#include "Photon/Realtime/Player.hpp"
#include "Photon/Realtime/Room.hpp"
#include "Photon/Realtime/AppSettings.hpp"

#include "Photon/Realtime/InRoomCallbacksContainer.hpp"
#include "Photon/Realtime/MatchMakingCallbacksContainer.hpp"
#include "Photon/Realtime/LoadBalancingClient.hpp"


#include "Photon/Realtime/ILobbyCallbacks.hpp"
#include "Photon/Realtime/IInRoomCallbacks.hpp"
#include "Photon/Realtime/IMatchmakingCallbacks.hpp"


#include "ExitGames/Client/Photon/Hashtable.hpp"

#include "GlobalNamespace/GorillaTagManager.hpp"
#include "GlobalNamespace/PhotonNetworkController.hpp"

using namespace GlobalNamespace;
using namespace Photon::Pun;
using namespace Photon::Realtime;

extern Logger& getLogger();

extern std::string patchForcedRegion;

static MapLoader::MonkeRoomManager* instance = nullptr;


RoomInfoData::RoomInfoData(RoomInfo* roomInfo)
{
    if (!roomInfo) return;

    name = roomInfo->get_Name() ? to_utf8(csstrtostr(roomInfo->get_Name())) : "";
    static Il2CppString* gameModeProperty = il2cpp_utils::createcsstr("gameMode", il2cpp_utils::StringType::Manual);

    Il2CppObject* property = roomInfo->customProperties->get_Item(gameModeProperty);

    if (property)
    {
        gameMode = to_utf8(csstrtostr(property->ToString()));
    }

    playerCount = roomInfo->get_PlayerCount();
}

RoomInfoData::RoomInfoData(RoomInfo* roomInfo, std::string region) : RoomInfoData(roomInfo) { this->region = region; };


namespace MapLoader
{
    void MonkeRoomManager::ctor()
    {
        //roomListCache = *il2cpp_utils::New<List<RoomRegionInfo*>*>();
        //roomListCache->Clear();
        instance = this;
        forcedRegion = "none";
        updateCounter() = 0;
        checkedRegions = nullptr;
    }

    void MonkeRoomManager::OnRoomListUpdate(List<RoomInfo*>* roomList)
    {
        using namespace GlobalNamespace;

        Il2CppString* currentRegionCS = PhotonNetwork::get_PhotonServerSettings()->AppSettings->FixedRegion;
        std::string currentRegion = currentRegionCS ? to_utf8(csstrtostr(currentRegionCS)) : "";

        if (forcedRegion == "none" && !checkedRegions)
        {
            // clear list because we are gonna add new stuff
            checkedRegions = new std::vector<std::string>();
            forcedRegion = currentRegion;
            Array<Il2CppString*>* regions = PhotonNetworkController::_get_instance()->serverRegions;
            for(int i= 0; i < regions->Length(); i++)
            {
                std::string region = regions->values[i] ? to_utf8(csstrtostr(regions->values[i])) : "";

                // we want a list of all regions except current region
                if (region != currentRegion) checkedRegions->push_back(region);
            }
        }

        getLogger().info("Sorting through %d rooms for region %s", roomList->get_Count(), currentRegion.c_str());
        int validRoomscount = 0;
        int oldRooms = 0;
        // foreach in roomlist
        for (int i = 0; i < roomList->get_Count(); i++)
        {
            RoomInfo* roomInfo = roomList->items->values[i];
            // if nullptr just skip cause crashy crash
            if (!roomInfo) continue;
            RoomInfoData data(roomInfo, currentRegion);

            // if there is no _ in the gamemode, it's not a custom map and can be skipped
            if (data.gameMode.find("_") == std::string::npos) continue;

            // will try to find at least 1 room with the same name in our cache
            bool found = false;
            int foundIndex = 0;
            for (int j = 0; j < roomListCache.size(); j++)
            {
                RoomInfoData& infoData = roomListCache[j];

                // if you find one that has the same name, just make it true and break since having even one makes it contain
                if (infoData.name == data.name)
                {
                    found = true;
                    foundIndex = j;
                    break;
                }
            }
            
            // if not found (count == 0)
            if (!found)
            {
                validRoomscount++;
            }
            // if found, replace it (remove)
            else
            {
                oldRooms++;
                roomListCache.erase(roomListCache.begin() + foundIndex);
            }
            
            // we always need to add a new one, so that is what we shall do
            roomListCache.push_back(data);
        }   

        // log new room and old room count for completeness sake
        getLogger().info("%d new Valid Rooms in %s, %d old rooms", validRoomscount, currentRegion.c_str(), oldRooms);

        // reset all room counts for current region
        RegionToMap::iterator regionIT = MapsInRegion.find(currentRegion);

        if (regionIT != MapsInRegion.end())
        {
            for (auto& p : regionIT->second)
            {
                p.second = 0;
            }
        }
        else
        {
            MapsInRegion[currentRegion] = (MapToCount){};
            regionIT = MapsInRegion.find(currentRegion);
        }


        // for each map data
        for (auto& data : roomListCache)
        {
            // if we are looking at a map that is not in the current checked region, skip, otherwise it would be counted towards each region
            if (data.region != currentRegion) continue;

            // get the map ID from the gamemode string, this is from MOD_ till DEFAULT
            int start = data.gameMode.find("MOD_") + 4;
            int end = data.gameMode.find("DEFAULT");
            std::string get = data.gameMode.substr(start, end - start);

            getLogger().info("GameMode: %s\n get: %s", data.gameMode.c_str(), get.c_str());
            
            // try to find if it exists-
           MapToCount::iterator it = regionIT->second.find(get);
            // if it does, add to the count there
            if (it != regionIT->second.end()) it->second += data.playerCount;
            // else just set it
            else regionIT->second[get] = data.playerCount;
        }

        
        
        if (roomList->get_Count() > 0)
        {
            // if we are not done checking regions
            if (checkedRegions->size() > 0)
            {
                // get new region
                std::string newRegion = checkedRegions->at(0);

                // remove first region from the list
                checkedRegions->erase(checkedRegions->begin());

                // set the value
                patchForcedRegion = newRegion;
                
                // disconnect makes it reconnect to master
                PhotonNetworkController::_get_instance()->AttemptDisconnect();
                //PhotonNetwork::Disconnect();
            }
            else if (checkedRegions->size() == 0 && forcedRegion != "none")
            {
                // set back to best ping region
                patchForcedRegion = forcedRegion;
                PhotonNetworkController::_get_instance()->AttemptDisconnect();
                forcedRegion = "none";
            }
        }

        updateCounter() ++;
        getLogger().info("update Counter: %d", updateCounter());
        // every n cycles, update all room data
        if (updateCounter() % 20 == 0)
        {
            updateCounter() = 0;
            getLogger().info("Updating all regions in room list cache");

            roomListCache.clear();

            checkedRegions->clear();
            delete(checkedRegions);
            checkedRegions = nullptr;

            forcedRegion = "none";

            PhotonNetworkController::_get_instance()->AttemptDisconnect();
        }
    }

    void MonkeRoomManager::OnConnectedToMaster()
    {
        // if in a lobby leave it
        if (PhotonNetwork::get_InLobby())
        {
            PhotonNetwork::LeaveLobby();
        }

        // join a lobby
        PhotonNetwork::JoinLobby();
    }

    MonkeRoomManager* MonkeRoomManager::get_instance()
    {
        return instance;
    }

    int MonkeRoomManager::PlayersOnMap(std::string mapName)
    {
        int count = 0;
        bool found = false;
        for (auto& p : MapsInRegion)
        {
            MapToCount::iterator it = p.second.find(mapName);
            if (it != p.second.end()) 
            {
                found = true;
                count += it->second;
            }
        }

        if (found) 
        {
            getLogger().info("Player count for map %s is %d", mapName.c_str(), count);
        }
        else
        {
            getLogger().info("Could not find player count for map %s", mapName.c_str());
        }

        return count;
    }

    std::string MonkeRoomManager::GetLobbyIfExists(std::string mapName)
    {
        // if there aren't even any rooms, let's save ourselves the trouble of looking for it
        if (roomListCache.size() == 0) return "";

        Il2CppString* currentRegionCS = PhotonNetworkController::_get_instance()->GetRegionWithLowestPing();
        std::string currentRegion = currentRegionCS ? to_utf8(csstrtostr(currentRegionCS)) : "";
        getLogger().info("Current Best Ping region: %s\nsearching in %lu rooms for map: %s", currentRegion.c_str(), roomListCache.size(), mapName.c_str());

        RoomInfoData* candidate = nullptr;
        bool wasBest = false;
        for (auto& i : roomListCache)
        {
            if (i.gameMode.find(mapName) != std::string::npos)
            {
                candidate = &i;
                // if region is not optimal, save it
                if (i.region == currentRegion)
                {
                    wasBest = true;
                    break;
                }
            }
        }

        if (candidate)
        {
            getLogger().info("Found room '%s' in region '%s'", candidate->name.c_str(), candidate->region.c_str());
            PhotonNetworkController::_get_instance()->ConnectToRegion(il2cpp_utils::createcsstr(candidate->region));
            return candidate->name;

            /*
            getLogger().info("Forcing region %s, this region was %s", candidate->region.c_str(), wasBest ? "In best Ping" : "Not in Best Ping");
            PhotonNetworkController::_get_instance()->ConnectToRegion(il2cpp_utils::createcsstr(candidate->region));
            */
            /*
            if (PhotonNetwork::get_InRoom())
                PhotonNetworkController::_get_instance()->AttemptDisconnect();
            else
                PhotonNetwork::Disconnect();
            */
        }

        return "";
    }
}