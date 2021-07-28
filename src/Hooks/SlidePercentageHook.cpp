#include "hooks.hpp"

#include "GorillaLocomotion/Player.hpp"
#include "GorillaLocomotion/Surface.hpp"

#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RaycastHit.hpp"

#include "Behaviours/SurfaceClimbSettings.hpp"

using namespace UnityEngine;
using namespace MapLoader;

MAKE_AUTO_HOOK_MATCH(Player_GetSlidePercentage, &GorillaLocomotion::Player::GetSlidePercentage, float, GorillaLocomotion::Player* self, RaycastHit raycastHit)
{
    Collider* collider = raycastHit.get_collider();
    GameObject* go = collider->get_gameObject();
    
    SurfaceClimbSettings* settings = go->GetComponent<SurfaceClimbSettings*>();
    
    static auto* get_sharedMesh = il2cpp_utils::FindMethodUnsafe("UnityEngine", "MeshCollider", "get_sharedMesh", 0);
    Mesh* sharedMesh = *il2cpp_utils::RunMethod<Mesh*>(collider, get_sharedMesh);

    if (settings)
    {
        GorillaLocomotion::Surface* surface = go->GetComponent<GorillaLocomotion::Surface*>();
        return surface->slipPercentage;
    }

    if (!collider || !sharedMesh)
    {
        return self->defaultSlideFactor;
    }
    
    if (!sharedMesh->get_isReadable())
    {
        return self->defaultSlideFactor;
    }
    return Player_GetSlidePercentage(self, raycastHit);
}