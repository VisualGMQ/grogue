#pragma once

#include "engine/engine.hpp"
#include "scripts/role_behavior.hpp"
#include "scripts/sword_behavior.hpp"

class StartScene final : public engine::Scene {
public:
    StartScene(const std::string& name): engine::Scene(name) {}

    void OnInit() override;
    void OnQuit() override;

private:
    void loadResources();
    void initNodes();
};