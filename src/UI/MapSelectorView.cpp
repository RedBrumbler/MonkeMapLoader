#include "UI/MapSelectorView.hpp"
#include "UI/MapView.hpp"
#include "monkecomputer/shared/GorillaUI.hpp"
#include "monkecomputer/shared/Helpers/SelectionHelper.hpp"
#include "monkecomputer/shared/Helpers/PageHelper.hpp"
#include "monkecomputer/shared/ViewLib/CustomComputer.hpp"
#include "Models/MapList.hpp"
#include "Behaviours/PreviewOrb.hpp"
#include "Behaviours/MapLoader.hpp"
#include "Behaviours/MonkeRoomManager.hpp"
#include "monkecomputer/shared/KeyExtension.hpp"

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Realtime/Room.hpp"
#include "Photon/Realtime/RoomInfo.hpp"
#include "Photon/Realtime/TypedLobby.hpp"

#include "Behaviours/RoomList.hpp"

DEFINE_TYPE(MapLoader, MapSelectorView);

extern Logger& getLogger();

using namespace GorillaUI;
using namespace Photon::Pun;
using namespace Photon::Realtime;

#define MOD_PAGE_SIZE 10

namespace MapLoader
{
    void MapSelectorView::Awake()
    {
        mapView = nullptr;

        selectionHandler = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true, true);
        pageSelectionHandler = new UISelectionHandler(EKeyboardKey::Left, EKeyboardKey::Right, EKeyboardKey::Enter, false, true);

        ((UISelectionHandler*)selectionHandler)->max = MOD_PAGE_SIZE;
        std::vector<MapInfo>& maps = MapList::get_maps();
        mapCount = maps.size();
        pageCount = PageHelper::GetPageAmount(maps, MOD_PAGE_SIZE);
        ((UISelectionHandler*)pageSelectionHandler)->max = pageCount;

        if (maps.size() < MOD_PAGE_SIZE)
        {
            ((UISelectionHandler*)selectionHandler)->max = maps.size();
        }
        getLogger().info("Got %d maps, on %d pages", mapCount, pageCount);
    }

    void MapSelectorView::DidActivate(bool firstActivation)
    {
        std::function<void(int)> fun = std::bind(&MapSelectorView::ShowMap, this, std::placeholders::_1);
        ((UISelectionHandler*)selectionHandler)->selectionCallback = fun;
        //PhotonNetwork::GetCustomRoomList(TypedLobby::_get_Default(), System::String::_get_Empty());

        Redraw();
        if (firstActivation)
        {
            if (mapCount != 0)
            {
                PreviewOrb::ChangeOrb(MapList::get_map(0));
            }
        }
    }

    void MapSelectorView::ShowMap(int index)
    {
        if (!mapView) mapView = GorillaUI::CreateView<MapView*>();
        if (!mapView)
        {
            getLogger().error("Map View Was Not Created!");
            return;
        }

        index += (MOD_PAGE_SIZE * ((UISelectionHandler*)pageSelectionHandler)->currentSelectionIndex);
        getLogger().info("Loading map %d", index);
        ((MapView*)mapView)->mapInfo = &MapList::get_map(index);
        if (computer)
        {
            GorillaUI::Components::ViewManager* activeViewManager = computer->activeViewManager;
            if (activeViewManager)
            {
                activeViewManager->ReplaceTopView(mapView);
            }
            else getLogger().error("active ViewManager was nullptr");
        }
        else getLogger().error("Computer instance was nullptr");
    }

    void MapSelectorView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawMaps();

        CustomComputer::Redraw();
    }
    
    void MapSelectorView::DrawHeader()
    {
        text += string_format("<color=#ffff00>== <color=#fdfdfd>Maps</color> ==</color> page [<] %d/%d [>]\n", ((UISelectionHandler*)pageSelectionHandler)->currentSelectionIndex + 1, pageCount);
    }
    
    void MapSelectorView::DrawMaps()
    {
        if (mapCount == 0)
        {
            text += "<size=40>\n\n";
            text += "    You have 0 maps installed! this means that the mod is currently useless...\n";
            text += "    Download maps from <color=#0000ff>monkemaphub.com</color>\n";
            text += "    and drop them into QuestPatcher, or put them in the following filepath on your quest:\n";
            text += "    <color=#fdfdfd>sdcard/ModData/com.AnotherAxiom.GorillaTag/Mods/MonkeMapLoader/CustomMaps</color>\n";
            text += "\n";
            text += "    Once you have put in these maps you either should restart the game, or use <color#fdfdfd>option 1</color> to reload the map list\n";
            text += "</size>";
            return;
        }
        
        std::vector<MapInfo> infos = PageHelper::GetPage(MapList::get_maps(), MOD_PAGE_SIZE, ((UISelectionHandler*)pageSelectionHandler)->currentSelectionIndex);
        ((UISelectionHandler*)selectionHandler)->max = infos.size();
        ((UISelectionHandler*)selectionHandler)->currentSelectionIndex = ((UISelectionHandler*)selectionHandler)->currentSelectionIndex >= infos.size() ? infos.size() - 1 : ((UISelectionHandler*)selectionHandler)->currentSelectionIndex;
        std::vector<std::string> mapNames = {};
        
        srand(time(NULL));
        
        // gather room info
        //List<RoomInfo*>* roominfos = RoomList::get_instance()->roomList;
        /*
        if (roominfos)
        {
            getLogger().info("Room infos: ");
            for (int i = 0; i < roominfos->size; i++)
            {
                RoomInfo* info = roominfos->items->values[i];

                Il2CppString* roomNameCS = info->get_Name();
                std::string roomName = to_utf8(csstrtostr(roomNameCS));

                int playerCount = info->get_PlayerCount();

                getLogger().info("\t room Name: %s,\n\t player count: %d", roomName.c_str(), playerCount);
            }
        }
        else
        {
            getLogger().info("Roominfos was nullptr");
        }
        */
        
        // what to draw
        for (auto m : infos)
        {
            // max display length is technically 41, but we want to also display user count so we need to reserve some space for that
            std::string display = m.packageInfo->descriptor.mapName.substr(0, 34);
            
            // fill the rest up with spaces
            for (int i = display.size(); i < 34; i++)
            {
                display += " ";
            }

            int userCount = MonkeRoomManager::PlayersOnMap(m.get_mapString());
            std::string countString = string_format("%d", userCount);
            // if we ever have more than 999 users in one map, just display it as >1k, since at that point the precision is not really required
            if (userCount > 999) 
            {
                countString = ">1k";
            }

            int countLength = countString.size();
            
            // display number in white
            display += string_format(" - <color=#fdfdfd>%s</color>", countString.c_str());
            
            mapNames.push_back(display);
        }

        SelectionHelper::DrawSelection(mapNames, ((UISelectionHandler*)selectionHandler)->currentSelectionIndex, text);
    }
    
    void MapSelectorView::OnKeyPressed(int key)
    {
        if (mapCount == 0) return;

        if (key == (int)EKeyboardKey::Option1)
        {
            MapList::Load();
            
            std::vector<MapInfo>& maps = MapList::get_maps();
            mapCount = maps.size();
            pageCount = PageHelper::GetPageAmount(maps, MOD_PAGE_SIZE);
            ((UISelectionHandler*)pageSelectionHandler)->max = pageCount;

            if (mapCount < MOD_PAGE_SIZE)
            {
                ((UISelectionHandler*)selectionHandler)->max = maps.size();
            }
        }
        else if (key == (int)EKeyboardKey::Option3)
        {
            Loader::get_instance()->UnloadMap();
        }

        char letter;
        if (KeyExtension::TryParseLetter((EKeyboardKey)key, letter))
        {
            std::vector<MapInfo>& maps = MapList::get_maps();
            int sel = 0;
            bool found = false;

            for (auto m : maps)
            {
                if (m.packageInfo->descriptor.mapName[0] == letter)
                {
                    found = true;
                    break;
                }
                sel++;
            }

            if (found)
            {
                int page = sel / MOD_PAGE_SIZE;
                sel = sel %= MOD_PAGE_SIZE;

                ((UISelectionHandler*)selectionHandler)->currentSelectionIndex = sel;
                ((UISelectionHandler*)pageSelectionHandler)->currentSelectionIndex = page;

                int index = ((UISelectionHandler*)selectionHandler)->currentSelectionIndex + (MOD_PAGE_SIZE * ((UISelectionHandler*)pageSelectionHandler)->currentSelectionIndex);
                if (index > mapCount - 1) index = mapCount - 1;
                PreviewOrb::ChangeOrb(MapList::get_map(index));
            }
        }

        int num;
        if (KeyExtension::TryParseNumber((EKeyboardKey)key, num))
        {
            std::vector<MapInfo>& maps = MapList::get_maps();
            int sel = 0;
            bool found = false;
            for (auto m : maps)
            {
                if (m.packageInfo->descriptor.mapName[0] == num + '0')
                {
                    found = true;
                    break;
                }
                sel++;
            }
            
            if (found)
            {
                int page = sel / MOD_PAGE_SIZE;
                sel %= MOD_PAGE_SIZE;

                ((UISelectionHandler*)selectionHandler)->currentSelectionIndex = sel;
                ((UISelectionHandler*)pageSelectionHandler)->currentSelectionIndex = page;

                int index = ((UISelectionHandler*)selectionHandler)->currentSelectionIndex + (MOD_PAGE_SIZE * ((UISelectionHandler*)pageSelectionHandler)->currentSelectionIndex);
                if (index > mapCount - 1) index = mapCount - 1;
                PreviewOrb::ChangeOrb(MapList::get_map(index));
            }
        }

        if (
                ((UISelectionHandler*)selectionHandler)->HandleKey((EKeyboardKey)key) ||
                ((UISelectionHandler*)pageSelectionHandler)->HandleKey((EKeyboardKey)key)
            )
        {
            int index = ((UISelectionHandler*)selectionHandler)->currentSelectionIndex + (MOD_PAGE_SIZE * ((UISelectionHandler*)pageSelectionHandler)->currentSelectionIndex);
            if (index > mapCount - 1)
            { 
                index = mapCount - 1;
            }
            PreviewOrb::ChangeOrb(MapList::get_map(index));
            this->Redraw();

        }
        else 
        {
            this->Redraw();
        }

        this->Redraw();
    }
}