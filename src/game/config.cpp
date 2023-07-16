#include "game/config.hpp"
#include "game_config_refl.hpp"
#include "game_types_refl.hpp"
#include "core_math_refl.hpp"

RaceProfDef::RaceProfDef(LuaManager& mgr, const std::string& filename): valid_(true) {
    auto lua = mgr.CreateSolitary(filename);
    auto root = lua.lua.get<std::optional<sol::table>>("Config");
    if (root) {
        auto data = serialize::DeserializeFromLua<RaceProfDefPOD>(root.value());
        if (data) {
            data_ = data.value();
        } else {
            valid_ = false;
        }
    } else {
        valid_ = false;
    }
}

RaceProfConfig::RaceProfConfig(LuaManager& mgr, const RaceProfDef& defs, const std::string& filename): valid_(false) {
    auto lua = mgr.CreateSolitary(filename);
    auto root = lua.lua.get<std::optional<sol::table>>("Config");
    if (root) {
        auto data = serialize::DeserializeFromLua<RacePOD>(root.value());
        if (data) {
            data.value().professions.resize(defs.Professions().size());
            // special treatment `professions` member
            auto profTable = root.value().get<std::optional<sol::table>>("professions");
            if (profTable) {
                valid_ = true;
                for (int i = 0; i < defs.Professions().size(); i++) {
                    auto& prof = defs.Professions()[i];
                    auto table = profTable.value().get<sol::table>(prof);
                    if (table) {
                        auto prop = serialize::DeserializeFromLua<MonsterPropertyRange>(table);
                        if (prop) {
                            data.value().professions[i] = prop.value();
                        }
                    } else {
                        LOGE("profession ", prof, " not defined");
                        valid_ = false;
                        break;
                    }
                }
                data_ = data.value();
            } 
        }
    }
}

GameConfig::GameConfig(LuaManager& luaMgr, FontManager& fontMgr, TilesheetManager& tsMgr, const std::string& configDir, const std::string& resDir): valid_(true) {
    raceProfDef_ = std::make_unique<RaceProfDef>(luaMgr, configDir + "race/" + "definitions.lua");
    if (!raceProfDef_->Valid()) {
        LOGE("definition.lua config read failed!");
        valid_ = false;
    } else {
        for (const auto& race : raceProfDef_->Races()) {
            std::string filename = configDir + "race/" + race + ".lua";
            auto config = std::make_shared<RaceProfConfig>(luaMgr, *raceProfDef_, filename);
            if (!config->Valid()) {
                LOGE("read ", race , " config failed");
                valid_ = false;
                break;
            } else {
                raceProfConfig_.push_back(std::move(config));
            }
        }
    }

    if (valid_) {
        itemConfig_ = std::make_unique<ItemConfig>(luaMgr, tsMgr, configDir + "items.lua");
        if (!itemConfig_->Valid()) {
            valid_ = false;
        }
    }

    if (valid_) {
        backpackUIConfig_ = std::make_unique<BackpackUIConfig>(luaMgr, configDir + "backpack_ui.lua");
        if (!backpackUIConfig_->Valid()) {
            valid_ = false;
        }
    }

    if (valid_) {
        auto lua = luaMgr.CreateSolitary(configDir + "game_conf.lua");
        auto root = lua.lua.get<std::optional<sol::table>>("Config");
        if (root) {
            auto conf = serialize::DeserializeFromLua<MiscGameConfig>(root.value());
            if (!conf) {
                valid_ = false;
            } else {
                misc_ = conf.value();

                // parse all actions
                // auto actions = root.value().get<sol::table>("actions");
                // if (actions.valid()) {
                //     for (const auto& [key, value] : actions) {
                //         misc_.actions[key.as<std::string>()] = value.as<std::string>();
                //     }
                // } else {
                //     LOGW("[Config]: no actions defined in game_conf.lua, did you forget?");
                // }

                auto uiFontPath = resDir + root.value().get<std::string>("ui_font");
                misc_.ui_font = fontMgr.Load(uiFontPath, misc_.ui_font_size);
            }
        } else {
            valid_ = false;
        }
    }
}

ItemConfig::ItemConfig(LuaManager& luaMgr, TilesheetManager& tsMgr, const std::string& filename): valid_(true) {
    auto lua = luaMgr.CreateSolitary(filename);
    auto root = lua.lua.get<std::optional<sol::table>>("Config");
    if (root) {
        auto totleWeight = root.value().get<std::optional<int>>("totle_weight");

        if (totleWeight) {
            totleWeight_ = totleWeight.value();
            assert(totleWeight != 0);
        }

        auto items = root.value().get<std::optional<sol::table>>("items");

        if (items) {
            for (auto item : items.value()) {
                ItemInfo info;

                auto name = item.first.as<std::string>();
                auto table = item.second.as<std::optional<sol::table>>();
                info.name = name;
                if (!table) {
                    LOGW(name, " must has a table");
                    continue;
                }
                auto pod = serialize::DeserializeFromLua<ItemPOD>(table.value());
                if (!pod) {
                    LOGW(name, " config invalid");
                    continue;
                }

                // assign POD to real struct
                for (const auto& operation : pod.value().operations) {
                    info.operations.push_back({operation, true});
                }

                info.architecture = pod.value().architecture;

                auto& tilesheet = tsMgr.Find(pod.value().sprite.tilesheet.name);
                auto tile = tilesheet.Get(pod.value().sprite.tilesheet.col, pod.value().sprite.tilesheet.row);
                info.sprite.image = tile.handle;
                info.sprite.sprite = Sprite::FromRegion(tile.region);
                info.weight = pod.value().weight;

                items_[name] = info;
                names_.push_back(name);
            }
        }
    }
}

BackpackUIConfig::BackpackUIConfig(LuaManager& mgr, const std::string& filename): valid_(true) {
    auto lua = mgr.CreateSolitary(filename);
    auto root = lua.lua.get<std::optional<sol::table>>("Config");
    if (root) {
        auto info = serialize::DeserializeFromLua<BackpackUIInfo>(root.value());
        if (!info) {
            valid_ = false;
        } else {
            info->col = (info->width - 2.0 * info->margin + info->padding) / (info->gridSize + info->padding);
            info->row = (info->height - 2.0 * info->margin + info->padding) / (info->gridSize + info->padding);
            info_ = info.value();
        }
    }
}