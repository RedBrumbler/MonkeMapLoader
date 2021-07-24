#include "Behaviours/SurfaceClimbSettings.hpp"

#include "UnityEngine/GameObject.hpp"
#include "GorillaLocomotion/Surface.hpp"

DEFINE_TYPE(MapLoader, SurfaceClimbSettings);

using namespace UnityEngine;
using namespace GorillaLocomotion;

namespace MapLoader
{
    void SurfaceClimbSettings::Start()
    {
        Surface* surface = GetComponent<Surface*>();

        if (!surface)
        {
            surface = get_gameObject()->AddComponent<Surface*>();
        }

        if (unClimbable)
            surface->slipPercentage = 1.0f;
        else 
            surface->slipPercentage = slipPercentage;

    }
}