#include "Behaviours/Teleporter.hpp"
#include "Behaviours/TagZone.hpp"
#include "Behaviours/Player.hpp"
#include "Behaviours/MapLoader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"

#include "UnityEngine/GameObject.hpp"

DEFINE_TYPE(MapLoader, Teleporter);

using namespace UnityEngine;

namespace MapLoader
{
    void Teleporter::ctor()
    {
        teleportPoints = *il2cpp_utils::New<List<Transform*>*>();
        teleportPoints->Clear();

        joinGameOnTeleport = false;
        isTeleporting = false;
        tagOnTeleport = false;
    }

    void Teleporter::Trigger(Collider* collider)
    {
        if (isTeleporting)
            return;

        isTeleporting = true;
        TeleportPlayer();
    }

    void Teleporter::TeleportPlayer()
    {
        if (teleporterType == TeleporterType::Map)
        {
            static Il2CppString* spawnPointContainerName = il2cpp_utils::createcsstr("SpawnPointContainer", il2cpp_utils::StringType::Manual);
            GameObject* spawnPointContainer = GameObject::Find(spawnPointContainerName);
            if (spawnPointContainer)
            {
                Transform* containerTransform = spawnPointContainer->get_transform();

                int childCount = containerTransform->get_childCount();
                teleportPoints->Clear();
                getLogger().info("Found %d children to teleport to!", childCount);
                for (int i = 0; i < childCount; i++)
                {
                    Transform* child = containerTransform->GetChild(i);
                    teleportPoints->Add(child);
                }
            }
        }

        if (teleportPoints->size == 0)
        {
            if (teleporterType == TeleporterType::Map)
            {
                getLogger().info("Even after trying to add spawnpoints, the array was empty");
                static Il2CppString* treeHomeTargetObjectName = il2cpp_utils::createcsstr("TreeHomeTargetObject", il2cpp_utils::StringType::Manual);
                GameObject* treeHomeTargetObject = GameObject::Find(treeHomeTargetObjectName);
                Transform* treeHomeTransform = treeHomeTargetObject->get_transform();
                teleportPoints->Add(treeHomeTransform);
            }
            else
            {
                getLogger().error("Teleporter point array was empty!");
                return;
            }
        }
        
        int index = teleportPoints->size > 1 ? rand() % teleportPoints->size : 0;
        Transform* dest = teleportPoints->items->values[index];
        
        // insttead of telepporting here

        if (tagOnTeleport) TagZone::TagLocalPlayer();
        if (joinGameOnTeleport) Loader::JoinGame();
        if (teleporterType == TeleporterType::Treehouse) 
        {
            getLogger().info("Resetting map properties becasue player went through a teleporter back");
            Loader::ResetMapProperties();
        }
        
        // teleport after the entire world has been shifted
        Player::TeleportPlayer(dest);

        isTeleporting = false;
    }
}