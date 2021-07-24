#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"

#include <functional>
#include <map>

typedef struct MapBehaviourRepresentation{
            UnityEngine::GameObject* first;
            std::string second;
        } MapBehaviourRepresentation;

typedef std::vector<MapBehaviourRepresentation> SerializeMap; 
typedef std::function<void(UnityEngine::GameObject*, std::string)> SerializedBehaviourCallback;

typedef struct RegisteredBehaviour {
    SerializeMap map;
    SerializedBehaviourCallback callback;
} RegisteredBehaviour;

DECLARE_CLASS_CODEGEN(MapLoader, MapDescriptor, UnityEngine::MonoBehaviour,
        DECLARE_INSTANCE_METHOD(void, Awake);
        DECLARE_INSTANCE_METHOD(void, Initialize);
        DECLARE_STATIC_METHOD(bool, CanBeDescriptor, UnityEngine::GameObject* go);
        DECLARE_INSTANCE_FIELD(float, gravity);

        public:
            std::map<std::string, RegisteredBehaviour> registeredBehaviours;

            void RegisterBehaviour(std::string filter, SerializedBehaviourCallback callback);
            bool CheckRegisteredCallbacks(UnityEngine::GameObject* first, std::string second);
)