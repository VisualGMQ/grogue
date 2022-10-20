#pragma once

#include "engine/engine.hpp"
#include "others/tilesheet_loader.hpp"
#include "others/object_reader.hpp"
#include "others/map_generate.hpp"

class StartScene final : public engine::Scene {
public:
    StartScene(const std::string& name): engine::Scene(name) {}

    void OnInit() override;
    void OnQuit() override;

private:
};