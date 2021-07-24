#include "Behaviours/MapDescriptor.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include <unordered_map>
#include <map>
#include "Behaviours/MapLoader.hpp"
#include "Behaviours/Teleporter.hpp"
#include "Behaviours/TagZone.hpp"
#include "Behaviours/ObjectTrigger.hpp"
#include "Behaviours/SurfaceClimbSettings.hpp"
#include "Behaviours/RoundEndActions.hpp"
#include "Behaviours/MovingPlatform.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Text.hpp"

#include "GlobalNamespace/GorillaScoreboardSpawner.hpp"
#include "GlobalNamespace/GorillaScoreBoard.hpp"

DEFINE_TYPE(MapLoader, MapDescriptor);

extern Logger& getLogger();

using namespace UnityEngine;

#define toMapIfContains(text, contains, map) \
if (text.find(contains) != std::string::npos) map.push_back({gameObject, text})

namespace MapLoader
{
    using TeleporterMap = std::map<std::string, Teleporter*>;
    using TriggerMap = std::map<std::string, ObjectTrigger*>;
    
    void MapDescriptor::Awake()
    {
        registeredBehaviours.clear();
        getLogger().info("Adding more instructions so it can definitely be hooked");
        getLogger().info("hm yes tasty instructions");
        getLogger().info("this should definitely be enough");
    }

    void MapDescriptor::RegisterBehaviour(std::string filter, SerializedBehaviourCallback callback)
    {
        registeredBehaviours[filter] = {{}, callback};
    }
    
    void MapDescriptor::Initialize()
    {
        Array<UI::Text*>* textComponents = GetComponentsInChildren<UI::Text*>(true);
        
        SerializeMap teleporters = {};
        SerializeMap teleportPoints = {};
        SerializeMap objectTriggers = {};
        SerializeMap triggerPoints = {};
        SerializeMap tagZones = {};
        SerializeMap surfaces = {};
        SerializeMap roundEndActions = {};
        SerializeMap actionObjects = {};

        SerializedBehaviourCallback fun = [](GameObject* go, std::string serialized) { go->AddComponent<MovingPlatform*>(); };
        RegisterBehaviour("MovingPlatform", fun);
        //SerializeMap platformObjects = {};
        
        getLogger().info("Handling all Text Components");
        for (int i = 0; i < textComponents->Length(); i++)
        {
            UI::Text* component = textComponents->values[i];
            Il2CppString* serializedCS = component->get_text();
            std::string serialized = to_utf8(csstrtostr(serializedCS));

            if (serialized.find("{") == std::string::npos || serialized.find("}") == std::string::npos) continue;
            GameObject* gameObject = component->get_gameObject();

            while (serialized.find("}, {") != std::string::npos)
            {

                int seperationpos = serialized.find("}, {");
                std::string subObj = serialized.substr(0, seperationpos + 1);
                serialized = serialized.erase(0, seperationpos + 3);

                     toMapIfContains(subObj, "TeleporterName", teleporters);
                else toMapIfContains(subObj, "TeleportPoint", teleportPoints);
                else toMapIfContains(subObj, "ObjectTriggerName", objectTriggers);
                else toMapIfContains(subObj, "TriggeredBy", triggerPoints);
                else toMapIfContains(subObj, "TagZone", tagZones);
                else toMapIfContains(subObj, "SurfaceClimbSettings", surfaces);
                else toMapIfContains(subObj, "RoundEndActions", roundEndActions);
                else toMapIfContains(subObj, "RoundEndAction", actionObjects);
                else if (CheckRegisteredCallbacks(gameObject, subObj)) {}
                else getLogger().error("Could not find object type for object with textcomponent:\n%s", subObj.c_str());
            }
            
                 toMapIfContains(serialized, "TeleporterName", teleporters);
            else toMapIfContains(serialized, "TeleportPoint", teleportPoints);
            else toMapIfContains(serialized, "ObjectTriggerName", objectTriggers);
            else toMapIfContains(serialized, "TriggeredBy", triggerPoints);
            else toMapIfContains(serialized, "TagZone", tagZones);
            else toMapIfContains(serialized, "SurfaceClimbSettings", surfaces);
            else toMapIfContains(serialized, "RoundEndActions", roundEndActions);
            else toMapIfContains(serialized, "RoundEndAction", actionObjects);
            else if (CheckRegisteredCallbacks(gameObject, serialized)) {}
            else getLogger().error("Could not find object type for object with textcomponent:\n%s", serialized.c_str());
        }

        // initialize teleporters
        TeleporterMap teleporterMap = {};
        static std::vector<Il2CppClass*> teleporterKlass = {classof(Teleporter*)};
        getLogger().info("Handling teleporters");
        for (auto t : teleporters)
        {
            Teleporter* tele = t.first->AddComponent<Teleporter*>();
            rapidjson::Document doc;
            doc.Parse(t.second.c_str());

            TouchType touchType = (TouchType)doc["TouchType"].GetInt();
            float delay = doc["Delay"].GetFloat();
            std::string teleporterName = doc["TeleporterName"].GetString();
            bool tagOnTeleport = doc["TagOnTeleport"].GetBool();

            teleporterMap[teleporterName] = tele;

            tele->touchType = touchType;
            tele->Delay = delay;
            tele->tagOnTeleport = tagOnTeleport;
            getLogger().info("\t%s", teleporterName.c_str());
        }    

        // initialize teleport points
        getLogger().info("Handling teleportPoints");
        for (auto t : teleportPoints)
        {
            Transform* transform = t.first->get_transform();
            rapidjson::Document doc;
            doc.Parse(t.second.c_str());

            std::string teleporter = doc["TeleportPoint"].GetString();

            teleporterMap[teleporter]->teleportPoints->Add(transform);
        }

        static Il2CppString* treeHomeTargetObjectName = il2cpp_utils::createcsstr("TreeHomeTargetObject", il2cpp_utils::StringType::Manual);
        GameObject* treeHomeTargetObject = GameObject::Find(treeHomeTargetObjectName);
        Transform* treeHomeTransform = treeHomeTargetObject->get_transform();

        // if there are 0 points, set type to treehouse
        for (auto t : teleporterMap)
        {
            int pointCount = t.second->teleportPoints->size;
            if (pointCount == 0) 
            {
                t.second->teleporterType = TeleporterType::Treehouse;
                t.second->teleportPoints->Add(treeHomeTransform);
            }
        }
        
        // initialize object triggers
        TriggerMap triggerMap = {};
        getLogger().info("Handling objectTriggers");
        for (auto t : objectTriggers)
        {
            ObjectTrigger* trigger = t.first->AddComponent<ObjectTrigger*>();
            rapidjson::Document doc;
            doc.Parse(t.second.c_str());

            std::string objectTriggerName = doc["ObjectTriggerName"].GetString();
            bool onlyTriggerOnce = doc["OnlyTriggerOnce"].GetBool();
            bool disableObject = doc["DisableObject"].GetBool();
            
            trigger->onlyTriggerOnce = onlyTriggerOnce;
            trigger->disableObject = disableObject;

            triggerMap[objectTriggerName] = trigger;

            getLogger().info("\t%s", objectTriggerName.c_str());
        }

        // initialize trigger points
        getLogger().info("Handling triggered");
        for (auto t : triggerPoints)
        {
            rapidjson::Document doc;
            doc.Parse(t.second.c_str());
            std::string trigger = doc["TriggeredBy"].GetString();
            
            triggerMap[trigger]->objectsToTrigger->Add(t.first);
            getLogger().info("\t%s", trigger.c_str());
        }

        // initialize tagZones
        getLogger().info("Handling TagZones");
        for (auto t : tagZones)
        {
            TagZone* zone = t.first->AddComponent<TagZone*>();
        }
        
        // initialize surfaces
        getLogger().info("Handling Surfaces");
        for (auto s : surfaces)
        {
            SurfaceClimbSettings* surface = s.first->AddComponent<SurfaceClimbSettings*>();
            rapidjson::Document doc;
            doc.Parse(s.second.c_str());
            
            bool unclimbable = doc["Unclimbable"].GetBool();
            float slipPercentage = doc["slipPercentage"].GetFloat();

            surface->unClimbable = unclimbable;
            surface->slipPercentage = slipPercentage;
            surface->Start();
        }
        
        getLogger().info("Handling Round end");
        std::vector<RoundEndActions*> endActions = {};
        for (auto r : roundEndActions)
        {
            RoundEndActions* roundEndAction = r.first->AddComponent<RoundEndActions*>();
            rapidjson::Document doc;
            doc.Parse(r.second.c_str());

            bool respawnOnRoundEnd = doc["RespawnOnRoundEnd"].GetBool();

            roundEndAction->respawnOnRoundEnd = respawnOnRoundEnd;
            endActions.push_back(roundEndAction);
        }

        for (auto a : actionObjects)
        {
            rapidjson::Document doc;
            doc.Parse(a.second.c_str());
            std::string action = doc["RoundEndAction"].GetString();
            for (auto endAction : endActions)
            {
                if (action == "Disable")
                {
                    endAction->objectsToDisable->Add(a.first);
                }
                else if (action == "Enable")
                {
                    endAction->objectsToEnable->Add(a.first);
                }
            }
        }

        /*
        for (auto p : platformObjects)
        {
            p.first->AddComponent<MovingPlatform*>();
        }
        */

        for (auto& r : registeredBehaviours)
        {
            for (auto& s : r.second.map)
            {
                r.second.callback(s.first, s.second);
            }
        }

        /*
        //setup scoreboard
        using namespace GlobalNamespace;
        static Il2cppString* customBoardName = il2cpp_utils::createcsstr("GorillaUI/CustomScoreboardAnchor", il2cpp_utils::StringType::Manual);
        static Il2CppString* boardPath = il2cpp_utils::createcsstr("GorillaUI/ForestScoreboardAnchor", il2cpp_utils::StringType::Manual);
        GameObject* originalBoard = GameObject::Find(boardPath);
        
        GorillaScoreboardSpawner* originalSpawner = originalBoard->GetComponent<GorillaScoreboardSpawner*>();
        GameObject* customBoard = Object::Instantiate(originalBoard);

        GorillaScoreboardSpawner* customSpawner = customBoard->GetComponent<GorillaScoreboardSpawner*>();
        Transform* customBoardTransform = customBoard->get_transform();

        customSpawner->gameType = il2cpp_utils::createcsstr("infection_MOD_" + Loader::lobbyName);
        customSpawner->scoreboardPrefab = originalSpawner->scoreboardPrefab;

        customBoardTransform->set_parent(originalBoard->get_transform()->get_parent(), false);

        customBoardTransform->set_position(Vector3(0.0f, 5002.0f, 0.0f));
        customBoardTransform->set_localEulerAngles(Vector3(0.0f, 180.0f, 0.0f));

        customBoard->SetActive(true);
        */
    }

    bool MapDescriptor::CheckRegisteredCallbacks(UnityEngine::GameObject* theGameObject, std::string serialized)
    {
        if (registeredBehaviours.size() == 0) return false;
        
        for (auto& b : registeredBehaviours)
        {
            // if the string contains the filter
            if (serialized.find(b.first) != std::string::npos)
            {
                b.second.map.push_back({theGameObject, serialized});
                return true;
            }            
        }

        return false;
    }

    bool MapDescriptor::CanBeDescriptor(GameObject* go)
    {
        Array<UI::Text*>* textComponents = go->GetComponentsInChildren<UI::Text*>(true);
        if (!textComponents) return false;
        for (int i = 0; i < textComponents->Length(); i++)
        {
            UI::Text* component = textComponents->values[i];
            Il2CppString* serializedCS = component->get_text();
            std::string serialized = to_utf8(csstrtostr(serializedCS));
            
            // first obj we find that can be a teleport or whatever means it was correct, then return true
            if (serialized.find("{") == std::string::npos || serialized.find("}") == std::string::npos) continue;
            return true;
        }

        // if none found return false
        return false;
    }
}