#include "hooks.hpp"
#include "GlobalNamespace/GorillaTagManager.hpp"
#include "GorillaLocomotion/Player.hpp"

#include "Hooks/PlayerMoveSpeedHook.hpp"
#include "shared-constants.hpp"

static bool needToSet = false;
using GorillaTagManager = GlobalNamespace::GorillaTagManager;
MAKE_AUTO_HOOK_MATCH(GorillaTagManager_Awake, &GorillaTagManager::Awake, void, GorillaTagManager* self)
{
    GorillaTagManager_Awake(self);

    if (MapLoader::PlayerMoveSpeedHook::get_needToSet())
    {
        self->slowJumpLimit = MapLoader::PlayerMoveSpeedHook::slowJumpLimit;
        self->fastJumpLimit = MapLoader::PlayerMoveSpeedHook::fastJumpLimit;
        self->slowJumpMultiplier = MapLoader::PlayerMoveSpeedHook::slowJumpMultiplier;
        self->fastJumpMultiplier = MapLoader::PlayerMoveSpeedHook::fastJumpMultiplier;
    }

    auto player = GorillaLocomotion::Player::_get__instance();
    player->maxJumpSpeed = self->slowJumpLimit;    
    player->jumpMultiplier = self->slowJumpMultiplier;    
}

namespace MapLoader
{
    bool PlayerMoveSpeedHook::needToSet = false;
    float PlayerMoveSpeedHook::slowJumpLimit = ::DefaultLimits.slowJumpLimit;
    float PlayerMoveSpeedHook::fastJumpLimit = ::DefaultLimits.fastJumpLimit;
    float PlayerMoveSpeedHook::slowJumpMultiplier = ::DefaultLimits.slowJumpMultiplier;
    float PlayerMoveSpeedHook::fastJumpMultiplier = ::DefaultLimits.fastJumpMultiplier;

    void PlayerMoveSpeedHook::SetSpeed(MapConfig& mapConfig)
    {
        slowJumpLimit = mapConfig.slowJumpLimit;
        fastJumpLimit = mapConfig.fastJumpLimit;
        slowJumpMultiplier = mapConfig.slowJumpMultiplier;
        fastJumpMultiplier = mapConfig.fastJumpMultiplier;
        needToSet = true;
    }
    
    bool PlayerMoveSpeedHook::get_needToSet()
    {
        if (needToSet) 
        {
            needToSet = false;
            return true;
        }
        return false;
    }
}

/*
void InstallPlayerMoveSpeedHooks(Logger& logger)
{
    SIMPLE_INSTALL_HOOK(GorillaTagManager_Awake);
}

MML_INSTALL_HOOKS(InstallPlayerMoveSpeedHooks);
*/