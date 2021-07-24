#pragma once

#include "custom-types/shared/macros.hpp"

namespace MapLoader {
    enum class TouchType {
        Any,
        Head,
        Hands
    };
}

#include "UnityEngine/Collider.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(MapLoader, GorillaMapTriggerBase, UnityEngine::MonoBehaviour,

        DECLARE_INSTANCE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);
        DECLARE_INSTANCE_METHOD(void, OnTriggerStay, UnityEngine::Collider* collider);
        DECLARE_INSTANCE_METHOD(void, OnTriggerExit, UnityEngine::Collider* collider);
        DECLARE_INSTANCE_METHOD(void, RunTrigger, UnityEngine::Collider* collider);
        DECLARE_INSTANCE_METHOD(bool, CheckColliderComponents, UnityEngine::Collider* collider);
        DECLARE_INSTANCE_FIELD(float, Delay);
        DECLARE_INSTANCE_FIELD(float, touchedTime);
        DECLARE_INSTANCE_FIELD(bool, isTriggering);
        DECLARE_INSTANCE_FIELD(UnityEngine::Collider*, collider);

    public:
        TouchType touchType = TouchType::Any;
)