#pragma once

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(MapLoader, MovingPlatform, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);
    DECLARE_METHOD(void, OnTriggerExit, UnityEngine::Collider* collider);
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, LateUpdate);
    DECLARE_METHOD(bool, isLocalPlayer, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, fakePosition);
    DECLARE_INSTANCE_FIELD(int, colliderCount);

    REGISTER_FUNCTION(
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(OnTriggerEnter);
        REGISTER_METHOD(OnTriggerExit);
        REGISTER_METHOD(Update);
        REGISTER_METHOD(LateUpdate);
        REGISTER_METHOD(isLocalPlayer);
        
        REGISTER_FIELD(fakePosition);
        REGISTER_FIELD(colliderCount);
    )
)