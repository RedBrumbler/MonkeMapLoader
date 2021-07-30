#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "hooks.hpp"

#include "beatsaber-hook/shared/utils/utils.h"
#include "quest-cosmetic-loader/shared/CosmeticLoader.hpp"
#include <thread>

#include "Behaviours/GorillaMapTriggerBase.hpp"
#include "Behaviours/ObjectTrigger.hpp"
#include "Behaviours/PreviewOrb.hpp"
#include "Behaviours/RotateByHand.hpp"
#include "Behaviours/RoundEndActions.hpp"
#include "Behaviours/SurfaceClimbSettings.hpp"
#include "Behaviours/TagZone.hpp"
#include "Behaviours/Teleporter.hpp"
#include "Behaviours/Player.hpp"
#include "Behaviours/MapDescriptor.hpp"
#include "Behaviours/MapLoader.hpp"
#include "Behaviours/RoomList.hpp"
#include "Behaviours/MonkeRoomManager.hpp"
#include "Behaviours/MovingPlatform.hpp"

#include "UI/MapView.hpp"
#include "UI/MapSelectorView.hpp"
#include "UI/MapSelectorViewManager.hpp"
#include "UI/MapLoaderUtilsView.hpp"

#include "monkecomputer/shared/Register.hpp"
#include "monkecomputer/shared/GorillaUI.hpp"

#include "Models/MapList.hpp"

#include "custom-types/shared/register.hpp"
#include "Utils/FileUtils.hpp"

#include "beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"

#include "GorillaLocomotion/Player.hpp"
#include "GorillaLocomotion/Surface.hpp"

#include "GlobalNamespace/VRRig.hpp"
#include "GlobalNamespace/GorillaComputer.hpp"
#include "GlobalNamespace/GorillaTagManager.hpp"
#include "GlobalNamespace/PhotonNetworkController.hpp"
#include "GlobalNamespace/GorillaQuitBox.hpp"

#include "UnityEngine/Animator.hpp"
#include "UnityEngine/Animations/AnimationPlayableOutput.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/RaycastHit.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/MeshCollider.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Pun/PhotonView.hpp"
#include "Photon/Pun/PhotonAnimatorView.hpp"

#include "Photon/Realtime/LoadBalancingClient.hpp"
#include "Photon/Realtime/Player.hpp"
#include "Photon/Realtime/Room.hpp"
#include "Photon/Realtime/RaiseEventOptions.hpp"
#include "Photon/Realtime/WebFlags.hpp"
#include "Photon/Realtime/ConnectionCallbacksContainer.hpp"

#include "ExitGames/Client/Photon/SendOptions.hpp"
#include "ExitGames/Client/Photon/Hashtable.hpp"

#include "gorilla-utils/shared/Callbacks/LobbyCallbacks.hpp"
#include "gorilla-utils/shared/Callbacks/ConnectionCallbacks.hpp"
#include "gorilla-utils/shared/Callbacks/MatchMakingCallbacks.hpp"
#include "gorilla-utils/shared/Utils/Room.hpp"
#include "gorilla-utils/shared/GorillaUtils.hpp"

//std::vector<void (*)(Logger& logger)> Hooks::installFuncs;


ModInfo modInfo;

Logger& getLogger()
{
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output);

using namespace CosmeticsLoader;
using namespace MapLoader;
using namespace UnityEngine;

MAKE_AUTO_HOOK_MATCH(Player_Awake, &GorillaLocomotion::Player::Awake, void, GorillaLocomotion::Player* self)
{
    Player_Awake(self);
    getLogger().info("self: %p", self);
    auto go = self->get_gameObject();
    getLogger().info("gameObject: %p", go);
    auto comp = go->AddComponent<MapLoader::Player*>();
    getLogger().info("component: %p", comp);
}

MAKE_AUTO_HOOK_MATCH(GorillaComputer_Start, &GlobalNamespace::GorillaComputer::Start, void, GlobalNamespace::GorillaComputer* self)
{
    if (!MonkeRoomManager::get_instance()) il2cpp_utils::New<MonkeRoomManager*>();

    GorillaComputer_Start(self);
    MapList::Load();

    GameObject* loaderGO = *il2cpp_utils::New<GameObject*>();
    loaderGO->AddComponent<Loader*>()->Initialize();

    Array<GlobalNamespace::GorillaQuitBox*>* quitBoxes = Resources::FindObjectsOfTypeAll<GlobalNamespace::GorillaQuitBox*>();

    for (int i = 0; i < quitBoxes->Length(); i++)
    {
        quitBoxes->values[i]->set_enabled(false);
    }
}

// forced region patch
std::string patchForcedRegion = "";
MAKE_AUTO_HOOK_MATCH(PhotonNetworkController_GetRegionWithLowestPing, &GlobalNamespace::PhotonNetworkController::GetRegionWithLowestPing, Il2CppString*, GlobalNamespace::PhotonNetworkController* self)
{
    Il2CppString* resultCS = PhotonNetworkController_GetRegionWithLowestPing(self);

    std::string result = resultCS ? to_utf8(csstrtostr(resultCS)) : "";

    // if we have a region we want to force and the region is not different, then return that
    if (patchForcedRegion != "" && result != patchForcedRegion)
    {
        result = patchForcedRegion;
        patchForcedRegion = "";
    }

    return il2cpp_utils::createcsstr(result);
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;

    modInfo = info;
}

extern "C" void load()
{
    getLogger().info("Loading mod...");
    GorillaUI::Innit();
    GorillaUtils::Innit();

    std::string mapDir = "/sdcard/ModData/com.AnotherAxiom.GorillaTag/Mods/MonkeMapLoader/CustomMaps/";
    FileUtils::mkdir(mapDir);

    Hooks::InstallHooks(getLogger());

    using namespace MapLoader;
    custom_types::Register::AutoRegister();

    GorillaUI::Register::RegisterViewManager<MapSelectorViewManager*>("Map Loader", VERSION);
    GorillaUI::Register::RegisterWatchView<MapLoaderUtilsView*>("Map Utils", VERSION);
    
    GorillaUtils::LobbyCallbacks::onRoomListUpdateEvent() += [](List<Photon::Realtime::RoomInfo*>* roomList) -> void {
        if (!MonkeRoomManager::get_instance()) il2cpp_utils::New<MonkeRoomManager*>();
        MonkeRoomManager::get_instance()->OnRoomListUpdate(roomList);
    };

    GorillaUtils::ConnectionCallbacks::onConnectedToMasterEvent() += []() -> void {
        if (!MonkeRoomManager::get_instance()) il2cpp_utils::New<MonkeRoomManager*>();
        MonkeRoomManager::get_instance()->OnConnectedToMaster();
    };

    GorillaUtils::MatchMakingCallbacks::onJoinedRoomEvent() += []() -> void {
        auto roomGamemode = GorillaUtils::Room::getRoomGameMode();
        if (roomGamemode)
        {
            if ((MapLoader::Loader::lobbyName != "") && ((*roomGamemode).find(MapLoader::Loader::lobbyName) != std::string::npos))
                MapLoader::Loader::ResetMapProperties();
        }
        else MapLoader::Loader::ResetMapProperties();
    };

    getLogger().info("Mod loaded!");
}

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output)
{
    Il2CppString* csString = scene.GetNameInternal(scene.m_Handle);
    if (!csString) return false;
    output = to_utf8(csstrtostr(csString));
    return true; 
}