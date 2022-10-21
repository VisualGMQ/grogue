#pragma once

#include "engine/engine.hpp"
#include "others/tilesheet_loader.hpp"
#include "others/object_reader.hpp"
#include "map/map_generate.hpp"
#include "system/maptile_render.hpp"
#include "system/transform_update.hpp"

class StartScene final : public engine::Scene {
public:
    StartScene(const std::string& name): engine::Scene(name) {}

    void OnInit() override;
    void OnQuit() override;

private:
    void initMap();
};