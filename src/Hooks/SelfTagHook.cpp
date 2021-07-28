#include "hooks.hpp"
#include "gorilla-utils/shared/CustomProperties/Player.hpp"
#include "gorilla-utils/shared/CustomProperties/Room.hpp"
#include "gorilla-utils/shared/Utils/Room.hpp"

#include "GlobalNamespace/GorillaTagManager.hpp"

#include "Photon/Realtime/Player.hpp"
#include "Photon/Realtime/RaiseEventOptions.hpp"
#include "Photon/Realtime/WebFlags.hpp"
#include "Photon/Pun/PhotonView.hpp"

#include "UnityEngine/Time.hpp"

using namespace UnityEngine;
using namespace Photon::Pun;
using namespace Photon::Realtime;

extern Logger& getLogger();

MAKE_AUTO_HOOK_MATCH(GorillaTagManager_ReportTag, &GlobalNamespace::GorillaTagManager::ReportTag, void, GlobalNamespace::GorillaTagManager* self, Photon::Realtime::Player* taggedPlayer, Photon::Realtime::Player* taggingPlayer)
{
    GorillaTagManager_ReportTag(self, taggedPlayer, taggingPlayer);
    
    PhotonView* photonView = self->get_photonView();
    
    bool IsMine = photonView->get_IsMine();
    bool equals = taggedPlayer ? taggedPlayer->Equals(taggingPlayer) : false;
    if (IsMine && equals)
    {
        RaiseEventOptions* raiseEventOptions = RaiseEventOptions::New_ctor();
        WebFlags* flags = WebFlags::New_ctor((uint8_t)1);
        raiseEventOptions->Flags = flags;

        bool isCurrentlyTag = self->isCurrentlyTag;

        Il2CppString* taggingPlayerID = taggingPlayer->get_UserId();
        Il2CppString* taggedPlayerID = taggedPlayer->get_UserId();

        if (isCurrentlyTag)
        {
            Photon::Realtime::Player* currentIt = self->currentIt;
            if (currentIt && !currentIt->Equals(taggedPlayer))
            {
                self->ChangeCurrentIt(taggedPlayer);
                self->lastTag = (double)UnityEngine::Time::get_time();
            }
            else getLogger().info("Player Was already it!");
        }
        else if (Time::get_time() > (self->lastTag + (double)self->tagCoolDown))
        {
            bool contains = self->currentInfected->Contains(taggedPlayer);
            
            if (!contains)
            {
                self->AddInfectedPlayer(taggedPlayer);
            }
            else getLogger().info("Player Was already infected!");
        }
    }
    else
    {
        getLogger().error("IsMine: %d, TaggedPlayer: %p, TaggingPlayer: %p", IsMine, taggedPlayer, taggingPlayer);
    }
}