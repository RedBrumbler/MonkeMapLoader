#pragma once

#include "custom-types/shared/macros.hpp"
#include "Behaviours/GorillaMapTriggerBase.hpp"

DECLARE_CLASS_CUSTOM(MapLoader, TagZone, MapLoader::GorillaMapTriggerBase,
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Trigger, Il2CppObject* collider);
    DECLARE_STATIC_METHOD(void, TagLocalPlayer);
    DECLARE_INSTANCE_FIELD(bool, canBeTagged);
)