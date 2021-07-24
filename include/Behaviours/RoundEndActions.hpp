#pragma once

#include "custom-types/shared/macros.hpp"

namespace MapLoader
{
    class RoundEndActions;
}

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(MapLoader, RoundEndActions, UnityEngine::MonoBehaviour,
    DECLARE_CTOR(ctor);
    DECLARE_INSTANCE_FIELD(bool, respawnOnRoundEnd);
    DECLARE_STATIC_METHOD(void, RoundEnd);
    DECLARE_INSTANCE_FIELD(List<UnityEngine::GameObject*>*, objectsToEnable);
    DECLARE_INSTANCE_FIELD(List<UnityEngine::GameObject*>*, objectsToDisable);
)