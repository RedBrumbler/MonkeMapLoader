#pragma once

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(MapLoader, MovingPlatform, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(void, OnTriggerExit, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, LateUpdate);
    DECLARE_INSTANCE_METHOD(bool, isLocalPlayer, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, fakePosition);
    DECLARE_INSTANCE_FIELD(int, colliderCount);
)