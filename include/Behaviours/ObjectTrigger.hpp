#pragma once

#include "custom-types/shared/macros.hpp"
#include "Behaviours/GorillaMapTriggerBase.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Collider.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

DECLARE_CLASS_CUSTOM(MapLoader, ObjectTrigger, MapLoader::GorillaMapTriggerBase,
    DECLARE_CTOR(ctor);
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, Trigger, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_FIELD(List<UnityEngine::GameObject*>*, objectsToTrigger);
    DECLARE_INSTANCE_FIELD(bool, disableObject);
    DECLARE_INSTANCE_FIELD(bool, onlyTriggerOnce);
    DECLARE_INSTANCE_FIELD(bool, triggered);
)