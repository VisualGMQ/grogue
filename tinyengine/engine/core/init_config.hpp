#pragma once

#include "engine/core/pch.hpp"
#include "engine/core/tool.hpp"

namespace engine {

class InitConfig final {
public:
    static InitConfig& Instance();

    void ParseFile(std::string_view name);

    std::string_view Title() const { return title_; }
    Size WindowSize() const { return size_; }
    bool Resizable() const { return resizable_; }
    bool IsUseSceneTree() const { return useSceneTree_; } 
    bool IsUseConsole() const { return useConsole_; }
    bool IsDrawColliderOutline() const { return isDrawColliderOutline_; }
    float PhysicalMinTimeStep() const { return physicalMinTimeStep_; }

private:
    std::string title_;
    Size size_;
    bool resizable_;
    bool useSceneTree_;
    bool useConsole_;
    bool isDrawColliderOutline_;
    float physicalMinTimeStep_;

    static std::unique_ptr<InitConfig> instance_;
};

}
