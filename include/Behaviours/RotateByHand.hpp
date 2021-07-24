#pragma once

#include "custom-types/shared/macros.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/Rigidbody.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(MapLoader, RotateByHand, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(void, OnTriggerStay, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(void, OnTriggerExit, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(bool, CheckIfValid, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(UnityEngine::Vector3, GetVector, UnityEngine::Collider* collider);

    DECLARE_INSTANCE_FIELD(UnityEngine::Vector3, midPoint);
    DECLARE_INSTANCE_FIELD(float, angle);
    DECLARE_INSTANCE_FIELD(UnityEngine::Vector3, startVector);
    DECLARE_INSTANCE_FIELD(UnityEngine::Rigidbody*, rigidBody);
    DECLARE_INSTANCE_FIELD(float, deltaAngle);
    DECLARE_INSTANCE_FIELD(float, previousAngle);
)