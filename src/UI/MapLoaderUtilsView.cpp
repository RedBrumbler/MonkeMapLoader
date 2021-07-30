#include "UI/MapLoaderUtilsView.hpp"
#include "Behaviours/Player.hpp"
#include "Behaviours/MapLoader.hpp"
#include "monkecomputer/shared/Helpers/SelectionHelper.hpp"
#include "monkecomputer/shared/ViewLib/MonkeWatch.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"

DEFINE_TYPE(MapLoader, MapLoaderUtilsView);

using namespace GorillaUI;
using namespace UnityEngine;

namespace MapLoader
{
    void MapLoaderUtilsView::Awake()
    {
        selectionHandler = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true, false);
        selectionHandler->max = 2;
    }

    void MapLoaderUtilsView::DidActivate(bool firstActivation)
    {
        std::function<void(int)> fun = std::bind(&MapLoaderUtilsView::OnConfirm, this, std::placeholders::_1);
        selectionHandler->selectionCallback = fun;
        Redraw();
    }

    void MapLoaderUtilsView::OnConfirm(int index)
    {
        switch (index)
        {
            case 0: // join map
                TeleportToMap();
                break;
            case 1: // leave map
                TeleportFromMap();
                break;
            default:
                break;
        }
    }

    void MapLoaderUtilsView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawBody();

        watch->Redraw();
    }

    void MapLoaderUtilsView::DrawHeader()
    {
        text += "<color=#ffff00>== <color=#fdfdfd>Map Utils</color> ==</color>\n\n";
    }

    void MapLoaderUtilsView::DrawBody()
    {
        std::vector<std::string> options = {
            "Join Map",
            "Exit Map"  
        };

        SelectionHelper::DrawSelection(options, selectionHandler->currentSelectionIndex, text);

        if (Loader::get_LoadData())
        {
            MapInfo info = Loader::get_LoadData()->info;
            
            
        }
    }

    void MapLoaderUtilsView::OnKeyPressed(int key)
    {
        selectionHandler->HandleKey((EKeyboardKey)key);
        Redraw();
    }

    void MapLoaderUtilsView::TeleportToMap()
    {
        if (Loader::isMoved() || !Loader::mapInstance()) return;
        static Il2CppString* spawnPointContainerName = il2cpp_utils::createcsstr("SpawnPointContainer", il2cpp_utils::StringType::Manual);
        GameObject* spawnPointContainer = GameObject::Find(spawnPointContainerName);

        if (!spawnPointContainer) return;
        Transform* container = spawnPointContainer->get_transform();

        int childCount = container->get_childCount();
        int index = childCount > 1 ? rand() % childCount : 0;

        Transform* dest = container->GetChild(index);

        Loader::JoinGame();
        Player::TeleportPlayer(dest);
    }

    void MapLoaderUtilsView::TeleportFromMap()
    {
        if (!Loader::isMoved() || !Loader::globalData) return;

        Transform* treePointTransform = nullptr;
        if (!Loader::globalData->bigTreePoint)
        {
            Il2CppString* treeHomeTargetObjectName = il2cpp_utils::createcsstr("TreeHomeTargetObject");
            Loader::globalData->bigTreePoint = GameObject::New_ctor();

            Loader::globalData->bigTreePoint->set_name(treeHomeTargetObjectName);
            treePointTransform = Loader::globalData->bigTreePoint->get_transform();

            Vector3 pos = {-66.0f, 12.3f, -83.0f};
            Quaternion rot = Quaternion::Euler(0.0f, -90.0f, 0.0f);

            treePointTransform->set_position(pos);
            treePointTransform->set_rotation(rot);

            Object::DontDestroyOnLoad(Loader::globalData->bigTreePoint);
        }
        else
        {
            treePointTransform = Loader::globalData->bigTreePoint->get_transform();
        }

        if (treePointTransform)
        {
            getLogger().info("Resetting map properties because player used the watch");
            Loader::ResetMapProperties();
            Player::TeleportPlayer(treePointTransform);
        }
    }
}