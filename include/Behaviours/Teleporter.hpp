#pragma once

#include "custom-types/shared/macros.hpp"
#include "Behaviours/GorillaMapTriggerBase.hpp"
#include <vector>

#include "UnityEngine/Collider.hpp"
#include "UnityEngine/Transform.hpp"

namespace MapLoader {
    enum class TeleporterType {
        Normal,
        Treehouse,
        Map
    };
}

DECLARE_CLASS_CUSTOM(MapLoader, Teleporter, MapLoader::GorillaMapTriggerBase,
    DECLARE_CTOR(ctor);
    DECLARE_INSTANCE_METHOD(void, Trigger, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(void, TeleportPlayer);
    DECLARE_INSTANCE_FIELD(bool, tagOnTeleport);
    DECLARE_INSTANCE_FIELD(bool, isTeleporting);
    DECLARE_INSTANCE_FIELD(bool, joinGameOnTeleport);
    DECLARE_INSTANCE_FIELD(List<UnityEngine::Transform*>*, teleportPoints);
    
    public:
        TeleporterType teleporterType = TeleporterType::Normal;
)