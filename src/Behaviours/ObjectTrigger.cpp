#include "Behaviours/ObjectTrigger.hpp"

DEFINE_TYPE(MapLoader, ObjectTrigger);

extern Logger& getLogger();
using namespace UnityEngine;

namespace MapLoader
{
    void ObjectTrigger::ctor()
    {
        objectsToTrigger = *il2cpp_utils::New<List<GameObject*>*>();
    }

    void ObjectTrigger::OnEnable()
    {
        for (int i = 0; i < objectsToTrigger->size; i++)
        {
            GameObject* objectToTrigger = objectsToTrigger->get_Item(i);

            objectToTrigger->SetActive(!disableObject);
            objectToTrigger->SetActive(disableObject);
        }

        triggered = false;
    }

    void ObjectTrigger::Trigger(Collider* collider)
    {
        if (triggered && onlyTriggerOnce)
            return;

        getLogger().info("Trigger %s", to_utf8(csstrtostr(get_gameObject()->get_name())).c_str());
        for (int i = 0; i < objectsToTrigger->size; i++)
        {
            GameObject* objectToTrigger = objectsToTrigger->get_Item(i);
            objectToTrigger->SetActive(disableObject);
            objectToTrigger->SetActive(!disableObject);
        }
        
        triggered = true;
    }
}