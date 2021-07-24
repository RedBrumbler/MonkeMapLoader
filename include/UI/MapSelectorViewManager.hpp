#pragma once

#include "monkecomputer/shared/EKeyboardKey.hpp"
#include "custom-types/shared/macros.hpp"
#include "monkecomputer/shared/ViewLib/ViewManager.hpp"
#include "monkecomputer/shared/ViewLib/View.hpp"

DECLARE_CLASS_CUSTOM(MapLoader, MapSelectorViewManager, GorillaUI::Components::ViewManager, 
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, NotifyOfKeyPress, int key);
    DECLARE_INSTANCE_METHOD(void, BackButtonWasPressed, GorillaUI::Components::View* view);
    
    DECLARE_INSTANCE_FIELD(GorillaUI::Components::View*, listView);
)