#include "hooks.hpp"

#include "Behaviours/MapLoader.hpp"
#include "Behaviours/RoundEndActions.hpp"

#include "GlobalNamespace/VRRig.hpp"
#include "GlobalNamespace/GorillaTagManager.hpp"
#include "GlobalNamespace/PhotonNetworkController.hpp"

#include "gorilla-utils/shared/Utils/Room.hpp"
#include "gorilla-utils/shared/CustomProperties/Room.hpp"

using namespace MapLoader;
using namespace Photon::Pun;
using namespace Photon::Realtime;

static double lastGameEnd = 0.0;
MAKE_AUTO_HOOK_MATCH(VRRig_PlayTagSound, &GlobalNamespace::VRRig::PlayTagSound, void, GlobalNamespace::VRRig* self, int soundIndex)
{
    using namespace GlobalNamespace;
    VRRig_PlayTagSound(self, soundIndex);

    PhotonNetworkController* photonNetworkController = PhotonNetworkController::_get_instance();
    if (photonNetworkController)
    {
        std::string currentGameType = GorillaUtils::Room::getRoomGameMode().value_or("");

        if (Loader::lobbyName != "" && currentGameType != "" && currentGameType.find(Loader::lobbyName) != std::string::npos)
        {
            GorillaTagManager* gorillaTagManager = GorillaTagManager::_get_instance();
            
            bool isCurrentlyTag = GorillaUtils::Room::GetProperty<bool>(gorillaTagManager->currentRoom, "isCurrentlyTag").value_or(false);
            double timeInfectedGameEnded = GorillaUtils::Room::GetProperty<double>(gorillaTagManager->currentRoom, "isCurrentlyTag").value_or(0.0);

            if (timeInfectedGameEnded > lastGameEnd)
            {
                lastGameEnd = timeInfectedGameEnded;
                RoundEndActions::RoundEnd();
            }
            else if (isCurrentlyTag) RoundEndActions::RoundEnd();
            else 
            {
                getLogger().info("Not calling game round end because time was not greater (%.2f <= %.2f) and isCurrentlyTag was false", timeInfectedGameEnded, lastGameEnd);
            }
        }
        else 
        {
            getLogger().error("Not running game end because lobby name was wrong,\nLooked for %s, had %s", Loader::lobbyName.c_str(), currentGameType.c_str());
        }
    }
    else
    {
        getLogger().error("PhotonNetworkController was nullptr");
    }
}