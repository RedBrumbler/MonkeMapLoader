#include "Behaviours/GorillaMapTriggerBase.hpp"

#include "UnityEngine/Time.hpp"
#include "UnityEngine/GameObject.hpp"

#include "GlobalNamespace/GorillaTriggerColliderHandIndicator.hpp"
#include "GorillaLocomotion/Player.hpp"

DEFINE_TYPE(MapLoader, GorillaMapTriggerBase);

extern Logger& getLogger();

using namespace UnityEngine;

namespace MapLoader
{
    void GorillaMapTriggerBase::OnTriggerEnter(Collider* collider)
    {
        if(isTriggering) return;
        if (!CheckColliderComponents(collider)) return;

        isTriggering = true;
        this->collider = collider;

        if (Delay == 0.0f)
        {
            RunTrigger(collider);
        }
        else touchedTime = 0.0f;
    }

    void GorillaMapTriggerBase::OnTriggerStay(Collider* collider)
    {
        if (Delay == 0.0f) return;
        if (!CheckColliderComponents(collider)) return;

        touchedTime += Time::get_fixedDeltaTime();

        if (touchedTime >= Delay)
        {
            touchedTime = 0.0f;
            RunTrigger(collider);
        }
    }

    void GorillaMapTriggerBase::OnTriggerExit(Collider* collider)
    {
        if (!CheckColliderComponents(collider)) return;
        if (this->collider != nullptr && this->collider != collider) return;

        isTriggering = false;
        this->collider = nullptr;
    }

    bool GorillaMapTriggerBase::CheckColliderComponents(Collider* collider)
    {
        GameObject* colliderGO = collider->get_gameObject();
        GlobalNamespace::GorillaTriggerColliderHandIndicator* handIndicator = colliderGO->GetComponentInParent<GlobalNamespace::GorillaTriggerColliderHandIndicator*>();
        GorillaLocomotion::Player* player = colliderGO->GetComponentInParent<GorillaLocomotion::Player*>();

        switch (touchType)
        {
            case TouchType::Any:
                if (!handIndicator && !player)
                    return false;
                break;
            case TouchType::Hands:
                if (!handIndicator)
                    return false;
                break;
            case TouchType::Head:
                if (!player || (player && handIndicator))
                    return false;
                break;
            default:
                return false;
        }
        return true;
    }

    void GorillaMapTriggerBase::RunTrigger(Collider* collider)
    {
        auto* trigger = il2cpp_functions::class_get_method_from_name(il2cpp_utils::ExtractClass(this), "Trigger", 1);
        if (trigger)
            il2cpp_utils::RunMethod(this, trigger, collider);

        isTriggering = false;
        this->collider = nullptr;
    }
}