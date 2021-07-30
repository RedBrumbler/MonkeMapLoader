
#include "hooks.hpp"
#include "Hooks/MatchMakingRegionHook.hpp"
#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Pun/ServerSettings.hpp"
#include "Photon/Realtime/AppSettings.hpp"
#include "UnityEngine/Random.hpp"

extern Logger& getLogger();

/// basically, this file makes the map loader join the first available room instead of a random region,
/// this way we always end up in an existing room instead of ending up in a new room if one exists

MAKE_AUTO_HOOK_MATCH(PhotonNetworkController_ProcessJoiningPublicRoomState, &GlobalNamespace::PhotonNetworkController::ProcessJoiningPublicRoomState, void, GlobalNamespace::PhotonNetworkController* self, GlobalNamespace::PhotonNetworkController::ConnectionEvent connectionEvent)
{
    if (MapLoader::MatchMakingRegionHook::ProcessJoiningPublicRoomState_Prefix(self, connectionEvent))
        PhotonNetworkController_ProcessJoiningPublicRoomState(self, connectionEvent);
}

namespace MapLoader
{
    using ConnectionEvent = GlobalNamespace::PhotonNetworkController::ConnectionEvent;
    using PhotonNetwork = Photon::Pun::PhotonNetwork;
    bool MatchMakingRegionHook::ProcessJoiningPublicRoomState_Prefix(GlobalNamespace::PhotonNetworkController* self, GlobalNamespace::PhotonNetworkController::ConnectionEvent connectionEvent)
    {
        switch (connectionEvent)
        {
            // Attempting to join a room, try and join a random room ignoring used regions
            case ConnectionEvent::OnDisconnected:
                {
                    if (self->joiningWithFriend || !self->pastFirstConnection) return true;
                    Array<int>* playersInRegion = reinterpret_cast<Array<int>*>(self->playersInRegion->Clone());
                    for (auto i : usedRegions)
                    {
                        playersInRegion->values[i] = 0;
                    }

                    // ported lemmings code
			    	float value = UnityEngine::Random::get_value();

			    	int num = 0;
                    int length = playersInRegion->Length();
			    	for (int i = 0; i < length; i++)
			    	{
			    		num += playersInRegion->values[i];
			    	}
			    	float num2 = 0.0f;
			    	int num3 = -1;
			    	while (num2 < value && num3 < length - 1)
			    	{
			    		num3++;
			    		num2 += (float)playersInRegion->values[num3] / (float)num;
			    	}
			    	PhotonNetwork::_get_photonServerSettings()->AppSettings->FixedRegion = self->serverRegions->values[num3];
			    	getLogger().info("joining past the first room, so we weighted randomly picked %s", to_utf8(csstrtostr(self->serverRegions->values[num3])).c_str());
                    // end of lemmings code


			    	// For some reason the first time this is called it just runs again, so don't ignore the region we just chose
			    	if (!firstRegion)
			    	{
			    		usedRegions.push_back(num3);
			    	}

			    	firstRegion = false;

			    	PhotonNetwork::ConnectUsingSettings();

                    return false;
                }
            // we failed to join a room in current region
            case ConnectionEvent::OnJoinRandomFailed:
                // if we we did not go through all the regions yet, disconnect and dont run the normal code
                if (usedRegions.size() < self->serverRegions->Length())
                {
                    PhotonNetwork::Disconnect();
                    return false;
                }
                // let it join a new room
                else return true;
            // we joined a room, so reset
            case ConnectionEvent::OnJoinedRoom:
                usedRegions.clear();
                firstRegion = true;
            default:
                return true;
        }
    }
}
