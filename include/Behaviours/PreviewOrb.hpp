#pragma once

#include "custom-types/shared/macros.hpp"
#include "Models/MapInfo.hpp"

#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

namespace MapLoader
{
    class PreviewOrb;
}

DECLARE_CLASS_CODEGEN(MapLoader, PreviewOrb, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Start);
    
    public:
        static inline PreviewOrb* instance = nullptr;
        static void ChangeOrb(UnityEngine::Color color, UnityEngine::Texture2D* texture);
        static void ChangeOrb(MapInfo& info);
)