#include "Behaviours/TagZone.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Pun/PhotonView.hpp"

#include "Photon/Realtime/Player.hpp"

#include "GlobalNamespace/GorillaTagManager.hpp"
#include "GlobalNamespace/GorillaGameManager.hpp"

DEFINE_TYPE(MapLoader, TagZone);


namespace MapLoader
{
    void TagZone::Awake()
    {
        canBeTagged = true;
    }

    void TagZone::Trigger(Il2CppObject* collider)
    {
        if (!canBeTagged)
            return;

        canBeTagged = false;
        TagLocalPlayer();

        std::thread ReTag([&](){
            for (int i = 0; i < 1000; i++)
            {
                usleep(1000);
            }

            canBeTagged = true;
        });

        ReTag.detach();
    }

    void TagZone::TagLocalPlayer()
    {
        using namespace Photon::Pun;
        using namespace Photon::Realtime;

        bool inRoom = PhotonNetwork::get_InRoom();
        if (inRoom)
        {
            GlobalNamespace::GorillaTagManager* gorillaTagManager = GlobalNamespace::GorillaTagManager::_get_instance();
            GlobalNamespace::GorillaGameManager* gameManager = gorillaTagManager->GetComponent<GlobalNamespace::GorillaGameManager*>();

            PhotonView* photonView = PhotonView::Get(gameManager);
            Player* localPlayer = PhotonNetwork::get_LocalPlayer();

            Array<Il2CppObject*>* sendArr = reinterpret_cast<Array<Il2CppObject*>*>(il2cpp_functions::array_new(classof(Il2CppObject*), 1));
            sendArr->values[0] = localPlayer;
            static Il2CppString* reportTagRPC = il2cpp_utils::createcsstr("ReportTagRPC", il2cpp_utils::StringType::Manual);

            photonView->RPC(reportTagRPC, RpcTarget::MasterClient, sendArr);
        }
    }
}