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
    DECLARE_METHOD(static void, ColorTreeTeleporter, UnityEngine::Color color);
    DECLARE_METHOD(static void, JoinGame);
    DECLARE_METHOD(static void, ForceRespawn);
    DECLARE_METHOD(static void, ResetMapProperties);
    DECLARE_METHOD(static Loader*, get_instance);
    
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, UnloadMap);
    DECLARE_METHOD(void, Initialize);
    DECLARE_METHOD(void, LoadData);
    DECLARE_METHOD(void, LoadBundle);
    DECLARE_METHOD(void, LoadScene);
    DECLARE_METHOD(static void, SceneComplete, void*, UnityEngine::AsyncOperation* sceneLoadRequest);
    DECLARE_METHOD(void, InitializeMap);
    DECLARE_METHOD(void, InitializeGlobalData);
    DECLARE_METHOD(void, ProcessMap, UnityEngine::GameObject* map);
    DECLARE_METHOD(void, ProcessChildren, UnityEngine::Transform* parent);
    DECLARE_METHOD(void, SetupCollisions, UnityEngine::GameObject* child);
    DECLARE_METHOD(void, FixLighting);

    public:
        static inline std::string lobbyName = "";
        static inline GlobalData* globalData = nullptr;
        static inline MapDescriptor* mapDescriptor = nullptr;
        void LoadMap(MapInfo info);
        static MapLoadData* get_LoadData();
    private:

        static MapLoadData* mapLoadData;
        static inline Loader* instance = nullptr;

    REGISTER_FUNCTION(
        REGISTER_METHOD(ctor);
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Update);
        REGISTER_METHOD(UnloadMap);
        REGISTER_METHOD(Initialize);
        REGISTER_METHOD(LoadData);
        REGISTER_METHOD(LoadBundle);
        REGISTER_METHOD(LoadScene);
        REGISTER_METHOD(SceneComplete);
        REGISTER_METHOD(InitializeMap);
        REGISTER_METHOD(InitializeGlobalData);
        REGISTER_METHOD(ColorTreeTeleporter);
        REGISTER_METHOD(JoinGame);
        REGISTER_METHOD(ForceRespawn);
        REGISTER_METHOD(ResetMapProperties);
        REGISTER_METHOD(ProcessMap);
        REGISTER_METHOD(ProcessChildren);
        REGISTER_METHOD(SetupCollisions);
        REGISTER_METHOD(FixLighting);
        REGISTER_METHOD(get_instance);
        REGISTER_FIELD(mapInstance);
        REGISTER_FIELD(isLoading);
        REGISTER_FIELD(isMoved);
    )
)