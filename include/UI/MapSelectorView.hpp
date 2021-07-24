#pragma once

#include "custom-types/shared/macros.hpp"
#include "monkecomputer/shared/ViewLib/View.hpp"
#include "monkecomputer/shared/InputHandlers/UISelectionHandler.hpp"

DECLARE_CLASS_CUSTOM(MapLoader, MapSelectorView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, ShowMap, int index);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawMaps);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_FIELD(GorillaUI::Components::View*, mapView);
    DECLARE_INSTANCE_FIELD(int, pageCount);
    DECLARE_INSTANCE_FIELD(int, mapCount);
    DECLARE_INSTANCE_FIELD(void*, selectionHandler);
    DECLARE_INSTANCE_FIELD(void*, pageSelectionHandler);
)