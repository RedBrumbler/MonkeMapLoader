#include "UI/MapSelectorViewManager.hpp"
#include "UI/MapSelectorView.hpp"
#include "UI/MapView.hpp"
#include "monkecomputer/shared/GorillaUI.hpp"
#include "monkecomputer/shared/EKeyboardKey.hpp"

DEFINE_TYPE(MapLoader, MapSelectorViewManager);

using namespace GorillaUI;

extern Logger& getLogger();
namespace MapLoader
{
    void MapSelectorViewManager::DidActivate(bool firstActivation)
    {
        if (firstActivation)
        {
            listView = CreateView<MapSelectorView*>();
            activeView = listView;
        }

        listView->computer = computer;
    }

    void MapSelectorViewManager::NotifyOfKeyPress(int key)
    {
        if (key == (int)EKeyboardKey::Back)
        {
            il2cpp_utils::RunMethod(this, "BackButtonWasPressed", activeView);
        }
        else
        {
            if (activeView)
            {
                il2cpp_utils::RunMethod(activeView, "OnKeyPressed", key);
            }
        }
    }

    void MapSelectorViewManager::BackButtonWasPressed(GorillaUI::Components::View* view)
    {
        if (view == listView)
        {
            if (parentViewManager)
                parentViewManager->DismissViewManager(this);
        }
        else 
        {
            ReplaceTopView(listView);
        }
    }
}