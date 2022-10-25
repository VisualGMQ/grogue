#include "engine/core/init_config.hpp"

namespace engine {

std::unique_ptr<InitConfig> InitConfig::instance_;

#ifndef DEBUG_MODE
#include "init_config.inc"
#endif

InitConfig& InitConfig::Instance() {
    if (!instance_) {
        instance_ = std::make_unique<InitConfig>();
        instance_->ParseFile("init.toml");
    }
    return *instance_;
}

void InitConfig::ParseFile(std::string_view name) {
    title_ = "TinyEngine v0.1.0";
    size_.w = 1024;
    size_.h = 720;
    resizable_ = false;
    physicalMinTimeStep_ = 0.01;

    toml::table tbl;
    try {
#ifdef DEBUG_MODE
        tbl = toml::parse_file("./init.toml");
#else
        tbl = toml::parse(InitConfigContent);
#endif
        title_ = tbl["title"].value_or(title_);
        size_.w = tbl["width"].value_or(size_.w);
        size_.h = tbl["height"].value_or(size_.h);
        resizable_ = tbl["resizable"].value_or(resizable_);
        physicalMinTimeStep_ = tbl["physical_min_time_step"].value_or(physicalMinTimeStep_);
    } catch (const toml::parse_error& err) {
        Logw("parse init.toml failed, skip config");
    }
}

}
