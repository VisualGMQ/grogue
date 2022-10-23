#pragma once

#include "engine/engine.hpp"
#include "others/tilesheet_loader.hpp"
#include "others/object_reader.hpp"
#include "map/map_generate.hpp"
#include "system/maptile_render.hpp"
#include "system/transform_update.hpp"
#include "system/sprite_render.hpp"
#include "system/mapobject_render.hpp"
#include "system/hint_arrow.hpp"
#include "others/localization.hpp"
#include "others/human_create.hpp"
#include "scripts/controller.hpp"
#include "others/data.hpp"
#include "system/controller_update.hpp"
#include "ui/backpack_panel.hpp"
#include "system/ui_render.hpp"

class StartScene final : public engine::Scene {
public:
    StartScene(const std::string& name): engine::Scene(name) {}

    void OnInit() override;
    void OnQuit() override;

private:
    void initMap();
};