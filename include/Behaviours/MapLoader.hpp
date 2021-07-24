#pragma once

#include "Models/MapInfo.hpp"
#include "Models/GlobalData.hpp"
#include "Behaviours/MapDescriptor.hpp"


#include "custom-types/shared/macros.hpp"

#include "UnityEngine/Color.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/AssetBundle.hpp"
#include "UnityEngine/AsyncOperation.hpp"
#include "UnityEngine/Transform.hpp"

namespace MapLoader
{
    enum class LoadState {
        LoadingData,
        LoadingBundle,
        LoadingScene,
        InitializingMap,
        FixLighting
    };

    struct MapLoadData
    {
        MapInfo info;
        bool moveNext = false;
        LoadState loadState = LoadState::LoadingData;
        std::vector<uint8_t> data = {};
        UnityEngine::AssetBundle* bundle = nullptr;
        Il2CppObject* root = nullptr;

        MapLoadData(MapInfo info) : info(info) {};
    };
}

DECLARE_CLASS_CODEGEN(MapLoader, Loader, UnityEngine::MonoBehaviour,
    DECLARE_STATIC_FIELD(UnityEngine::GameObject*, mapInstance);
    DECLARE_STATIC_FIELD(bool, isLoading);
    DECLARE_STATIC_FIELD(bool, isMoved);

    DECLARE_CTOR(ctor);
    DECLARE_STATIC_METHOD(void, ColorTreeTeleporter, UnityEngine::Color color);
    DECLARE_STATIC_METHOD(void, JoinGame);
    DECLARE_STATIC_METHOD(void, ForceRespawn);
    DECLARE_STATIC_METHOD(void, ResetMapProperties);
    DECLARE_STATIC_METHOD(Loader*, get_instance);
    
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, UnloadMap);
    DECLARE_INSTANCE_METHOD(void, Initialize);
    DECLARE_INSTANCE_METHOD(void, LoadData);
    DECLARE_INSTANCE_METHOD(void, LoadBundle);
    DECLARE_INSTANCE_METHOD(void, LoadScene);
    DECLARE_STATIC_METHOD(void, SceneComplete, void*, UnityEngine::AsyncOperation* sceneLoadRequest);
    DECLARE_INSTANCE_METHOD(void, InitializeMap);
    DECLARE_INSTANCE_METHOD(void, InitializeGlobalData);
    DECLARE_INSTANCE_METHOD(void, ProcessMap, UnityEngine::GameObject* map);
    DECLARE_INSTANCE_METHOD(void, ProcessChildren, UnityEngine::Transform* parent);
    DECLARE_INSTANCE_METHOD(void, SetupCollisions, UnityEngine::GameObject* child);
    DECLARE_INSTANCE_METHOD(void, FixLighting);

    public:
        static inline std::string lobbyName = "";
        static inline GlobalData* globalData = nullptr;
        static inline MapDescriptor* mapDescriptor = nullptr;
        void LoadMap(MapInfo info);
        static MapLoadData* get_LoadData();
    private:

        static MapLoadData* mapLoadData;
        static inline Loader* instance = nullptr;
)