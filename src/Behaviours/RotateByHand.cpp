#include "Behaviours/RotateByHand.hpp"

DEFINE_TYPE(MapLoader, RotateByHand);

#include "UnityEngine/Collider.hpp"
#include "UnityEngine/Bounds.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Time.hpp"

#include "GlobalNamespace/GorillaTriggerColliderHandIndicator.hpp"

using namespace UnityEngine;

extern Logger& getLogger();
namespace MapLoader
{
    void RotateByHand::Awake()
    {
        midPoint = (Vector3){0.0f, 0.0f, 0.0f};
        angle = 0;
        deltaAngle = 0;
        previousAngle = 0;

        Collider* collider = get_gameObject()->GetComponent<Collider*>();
        midPoint = collider->get_bounds().m_Center;

        // Rigidbody Stuff
        rigidBody = get_gameObject()->GetComponent<Rigidbody*>();
        if (!rigidBody) rigidBody = get_gameObject()->AddComponent<Rigidbody*>();

        rigidBody->set_useGravity(false);
        rigidBody->set_angularDrag(0.8f);
        getLogger().info("RotateByHand Awake end");
    }

    void RotateByHand::OnTriggerEnter(Collider* collider)
    {
        if (!CheckIfValid(collider)) return;
        angle = get_transform()->get_eulerAngles().y;

        startVector = GetVector(collider);
    }

    void RotateByHand::OnTriggerStay(Collider* collider)
    {
        if (!CheckIfValid(collider)) return;
        Vector3 currentVector = GetVector(collider);
        Vector3 angles = get_transform()->get_eulerAngles();

        float vectorAngle = Vector3::SignedAngle(currentVector, startVector, Vector3::get_up());

        deltaAngle = vectorAngle - previousAngle;
        previousAngle = vectorAngle;
    }

    void RotateByHand::OnTriggerExit(Collider* collider)
    {
        if (!CheckIfValid(collider)) return;

        float fixedDeltaTime = Time::get_fixedDeltaTime();
        float angularVelocityFloat = (-deltaAngle / fixedDeltaTime);
        Vector3 angularVelocity = {0.0f, angularVelocityFloat, 0.0f};

        rigidBody->set_angularVelocity(angularVelocity);
    }

    bool RotateByHand::CheckIfValid(Collider* collider)
    {
        GlobalNamespace::GorillaTriggerColliderHandIndicator* handIndicator = collider->GetComponentInParent<GlobalNamespace::GorillaTriggerColliderHandIndicator*>();
        if (!handIndicator) return false;
        if (midPoint.x == 0.0f &&
            midPoint.y == 0.0f &&
            midPoint.z == 0.0f)
            return false;
        return true;
    }

    Vector3 RotateByHand::GetVector(Collider* collider)
    {
        Vector3 closestPoint = collider->ClosestPoint(midPoint);
        Vector3 newStart = -midPoint;
        newStart.x += closestPoint.x;
        newStart.y += closestPoint.y;
        newStart.z += closestPoint.z;

        return newStart;
    }
}