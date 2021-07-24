#pragma once

#include "custom-types/shared/macros.hpp"
#include "monkecomputer/shared/ViewLib/View.hpp"
#include "monkecomputer/shared/InputHandlers/UISelectionHandler.hpp"

DECLARE_CLASS_CUSTOM(MapLoader, MapLoaderUtilsView, GorillaUI::Components::View,
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawBody);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_METHOD(void, OnConfirm, int index);
    DECLARE_STATIC_METHOD(void, TeleportToMap);
    DECLARE_STATIC_METHOD(void, TeleportFromMap);
    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
)