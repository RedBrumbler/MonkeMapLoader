#include "Behaviours/RoundEndActions.hpp"
#include "Behaviours/MapLoader.hpp"

#include "UnityEngine/Resources.hpp"

DEFINE_TYPE(MapLoader::RoundEndActions);

#include "MapEventCallbacks.hpp"

using namespace UnityEngine;

namespace MapLoader
{
    void RoundEndActions::ctor()
    {
        objectsToEnable = *il2cpp_utils::New<List<GameObject*>*>();
        objectsToDisable = *il2cpp_utils::New<List<GameObject*>*>();
    }

    void RoundEndActions::RoundEnd()
    {
        Array<RoundEndActions*>* endActions = Resources::FindObjectsOfTypeAll<RoundEndActions*>();
        if (!endActions) return;

        MapEventCallbacks::OnRoundEnd();
        
        for (int i = 0; i < endActions->Length(); i++)
        {
            RoundEndActions* action = endActions->values[i];
            getLogger().info("Found action %d", i);
            for (int j = 0; j < action->objectsToDisable->size; j++)
            {
                action->objectsToDisable->items->values[j]->SetActive(true);
                action->objectsToDisable->items->values[j]->SetActive(false);
            }

            for (int j = 0; j < action->objectsToEnable->size; j++)
            {
                action->objectsToEnable->items->values[j]->SetActive(false);
                action->objectsToEnable->items->values[j]->SetActive(true);
            }

            if (action->respawnOnRoundEnd)
            {
                Loader::ForceRespawn();
            }
        }
    }
}