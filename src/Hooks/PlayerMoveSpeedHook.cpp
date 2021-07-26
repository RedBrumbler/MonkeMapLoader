#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "hooks.hpp"
#include "GlobalNamespace/GorillaTagManager.hpp"
#include "GorillaLocomotion/Player.hpp"

#include "Hooks/PlayerMoveSpeedHook.hpp"

static bool needToSet = false;
using GorillaTagManager = GlobalNamespace::GorillaTagManager;
MAKE_AUTO_HOOK_MATCH(GorillaTagManager_Awake, &GorillaTagManager::Awake, void, GorillaTagManager* self)
{
    GorillaTagManager_Awake(self);

    MapLoader::MapConfig* config;
    if (MapLoader::PlayerMoveSpeedHook::get_needToSet() && (config = MapLoader::PlayerMoveSpeedHook::get_config()))
    {
        self->slowJumpLimit = config->slowJumpLimit;
        self->fastJumpLimit = config->fastJumpLimit;
        self->slowJumpMultiplier = config->slowJumpMultiplier;
        self->fastJumpMultiplier = config->fastJumpMultiplier;
    }

    auto player = GorillaLocomotion::Player::_get__instance();
    player->maxJumpSpeed = self->slowJumpLimit;    
    player->jumpMultiplier = self->slowJumpMultiplier;    
}

namespace MapLoader
{
    bool PlayerMoveSpeedHook::needToSet = false;
    MapConfig* PlayerMoveSpeedHook::config = nullptr;

    void PlayerMoveSpeedHook::SetSpeed(MapConfig* mapConfig)
    {
        config = mapConfig;
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

    MapConfig* PlayerMoveSpeedHook::get_config()
    {
        return config;
    }
}

/*
void InstallPlayerMoveSpeedHooks(Logger& logger)
{
    SIMPLE_INSTALL_HOOK(GorillaTagManager_Awake);
}

MML_INSTALL_HOOKS(InstallPlayerMoveSpeedHooks);
*/