#include "Behaviours/MapNetworkJoinTrigger.hpp"
#include "UnityEngine/GameObject.hpp"
#include "System/String.hpp"
#include "GlobalNamespace/PhotonNetworkController.hpp"
#include "GlobalNamespace/GorillaLevelScreen.hpp"

DEFINE_TYPE(MapLoader, MapNetworkJoinTrigger);

using namespace UnityEngine;
namespace MapLoader
{
    void MapNetworkJoinTrigger::ctor()
    {
        instance = this;
        makeSureThisIsDisabled = reinterpret_cast<Array<GameObject*>*>(il2cpp_functions::array_new(classof(GameObject*), 0));
        makeSureThisIsEnabled = reinterpret_cast<Array<GameObject*>*>(il2cpp_functions::array_new(classof(GameObject*), 0));
        gameModeName = Il2CppString::_get_Empty();
        photonNetworkController = GlobalNamespace::PhotonNetworkController::_get_instance();
        componentTarget = nullptr;

        joinScreens = reinterpret_cast<Array<GlobalNamespace::GorillaLevelScreen*>*>(il2cpp_functions::array_new(classof(GlobalNamespace::GorillaLevelScreen*), 0));
        leaveScreens = reinterpret_cast<Array<GlobalNamespace::GorillaLevelScreen*>*>(il2cpp_functions::array_new(classof(GlobalNamespace::GorillaLevelScreen*), 0));

        triggerBoxOnce = false;
        sliderValues = reinterpret_cast<Array<float>*>(il2cpp_functions::array_new(classof(float), 0));
    }

    MapNetworkJoinTrigger* MapNetworkJoinTrigger::get_instance()
    {
        return instance;
    }
}