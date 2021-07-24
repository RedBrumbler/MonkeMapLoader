#pragma once

#include "custom-types/shared/macros.hpp"
#include "GlobalNamespace/GorillaNetworkJoinTrigger.hpp"
#include "GlobalNamespace/GorillaTriggerBox.hpp"

DECLARE_CLASS_CODEGEN(MapLoader, MapNetworkJoinTrigger, GlobalNamespace::GorillaNetworkJoinTrigger,
    DECLARE_CTOR(ctor);
    DECLARE_STATIC_METHOD(MapNetworkJoinTrigger*, get_instance);

    private:
        static inline MapNetworkJoinTrigger* instance = nullptr;
)