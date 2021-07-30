#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(MapLoader, Player, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, destination);
    DECLARE_INSTANCE_FIELD(bool, isTeleporting);

    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_STATIC_METHOD(Player*, get_instance);
    DECLARE_STATIC_METHOD(void, TeleportPlayer, UnityEngine::Transform* destination);
    DECLARE_STATIC_METHOD(void, Offset, UnityEngine::Vector3 offset);
    DECLARE_STATIC_METHOD(UnityEngine::Vector3, get_localizedPosition);
    public:
        static inline Player* instance = nullptr;
)