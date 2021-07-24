#include "Behaviours/MovingPlatform.hpp"

#include "Behaviours/Player.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/PrimitiveType.hpp"
#include "UnityEngine/Object.hpp"

#include "GlobalNamespace/GorillaTriggerColliderHandIndicator.hpp"
#include "GorillaLocomotion/Player.hpp"

DEFINE_TYPE(MapLoader, MovingPlatform);

using namespace UnityEngine;

extern Logger& getLogger();

namespace MapLoader
{
    void MovingPlatform::Awake()
    {
        static Il2CppString* fakePositionName = il2cpp_utils::createcsstr("fakePosition", il2cpp_utils::StringType::Manual);

        fakePosition = get_transform()->Find(fakePositionName);

        if (!fakePosition)
        {
            fakePosition = GameObject::New_ctor()->get_transform();
            fakePosition->SetParent(get_transform());
            fakePosition->get_gameObject()->set_name(fakePositionName);
        }

        colliderCount = 0;
    }

    void MovingPlatform::OnTriggerEnter(Collider* collider)
    {
        Il2CppString* nameCS = collider->get_gameObject()->get_name();    

        if (isLocalPlayer(collider))
        {
            colliderCount++;  
        }
    }

    void MovingPlatform::OnTriggerExit(Collider* collider)
    {
        Il2CppString* nameCS = collider->get_gameObject()->get_name();    
        if (isLocalPlayer(collider))
        {
            colliderCount--;
            if (colliderCount < 0) colliderCount = 0;
        }
    }

    void MovingPlatform::Update()
    {
        if (colliderCount <= 0) return;
        fakePosition->set_position(Player::get_localizedPosition());
    }

    void MovingPlatform::LateUpdate()
    {
        if (colliderCount <= 0) return;
        Vector3 delta = fakePosition->get_position() - Player::get_localizedPosition();
        Player::Offset(delta);
    }

    bool MovingPlatform::isLocalPlayer(Collider* collider)
    {
        GameObject* colliderGO = collider->get_gameObject();
        if (colliderGO->GetComponentInParent<GlobalNamespace::GorillaTriggerColliderHandIndicator*>()) return true;
        else if (colliderGO->GetComponentInParent<GorillaLocomotion::Player*>()) return true;

        return false;
    }
}