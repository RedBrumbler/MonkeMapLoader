#pragma once

#include "custom-types/shared/macros.hpp"
#include "monkecomputer/shared/ViewLib/View.hpp"
#include "monkecomputer/shared/InputHandlers/UISelectionHandler.hpp"
#include "Models/MapInfo.hpp"

DECLARE_CLASS_CUSTOM(MapLoader, MapView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Load);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawMap);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_FIELD(bool, loaded);
    DECLARE_INSTANCE_FIELD(void*, mapInfo);
    DECLARE_INSTANCE_FIELD(bool, isUpdated);

    public:
        std::vector<std::string> missingModIDs = {};
)