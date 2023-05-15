#include "game/config.hpp"

// generate parse funcs

// clang-format off
DeclareParseFunc(MonsterProperty)
    Field(hp, int)
    Field(mp, int)
    Field(strength, int)
    Field(intelligence, int)
    Field(outsight, int)
    Field(constitution, int)
    Field(agility, int)
    Field(nutrition, int)
EndDeclareParseFunc()

DeclareParseFunc(RaceProfDefPOD)
    DynArrayField(races, std::string)
    DynArrayField(professions, std::string)
EndDeclareParseFunc()

DeclareParseFunc(MonsterPropertyRange)
    ArrayField(hp, int, 2)
    ArrayField(mp, int, 2)
    ArrayField(strength, int, 2)
    ArrayField(intelligence, int, 2)
    ArrayField(outsight, int, 2)
    ArrayField(constitution, int, 2)
    ArrayField(agility, int, 2)
    ArrayField(nutrition, int, 2)
EndDeclareParseFunc()

DeclareParseFunc(RacePOD)
    Field(name, std::string)
    ObjField(basic, MonsterProperty)
    ObjField(max, MonsterProperty)
EndDeclareParseFunc()

DeclareParseFunc(TilesheetPOD)
    Field(name, std::string)
    Field(row, int)
    Field(col, int)
EndDeclareParseFunc()

DeclareParseFunc(SpritePOD)
    ArrayField(color, int, 4)
    ObjField(tilesheet, TilesheetPOD)
EndDeclareParseFunc()

DeclareParseFunc(Material)
    Field(opaque, float)
    Field(solid, float)
    Field(lumen, float)
EndDeclareParseFunc()

DeclareParseFunc(ItemPOD)
    Field(name, std::string)
    Field(weight, int)
    DynArrayField(operations, std::string)
    ObjField(sprite, SpritePOD)
    ObjField(material, Material)
EndDeclareParseFunc()
// clang-format on

// config class

RaceProfDef::RaceProfDef(LuaManager& mgr, const std::string& filename): valid_(true) {
    auto lua = mgr.CreateSolitary(filename);
    auto root = lua.lua.get<std::optional<sol::table>>("Config");
    if (root) {
        auto data = ParseRaceProfDefPOD(root.value());
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
        auto data = ParseRacePOD(root.value());
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
                        auto prop = ParseMonsterPropertyRange(table);
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

GameConfig::GameConfig(LuaManager& luaMgr, TilesheetManager& tsMgr, const std::string& configDir): valid_(true) {
    raceProfDef_ = std::make_unique<RaceProfDef>(luaMgr, configDir + "race/" + "definitions.lua");
    if (!raceProfDef_->Valid()) {
        LOGE("definition.lua config read failed!");
        valid_ = false;
    } else {
        for (const auto& race : raceProfDef_->Races()) {
            std::string filename = configDir + "race/" + race + ".lua";
            auto config = std::make_unique<RaceProfConfig>(luaMgr, *raceProfDef_, filename);
            if (!config->Valid()) {
                LOGE("read ", race , " config failed");
                valid_ = false;
                break;
            }
        }
    }

    if (valid_) {
        itemConfig_ = std::make_unique<ItemConfig>(luaMgr, tsMgr, configDir + "items.lua");
        if (!itemConfig_->Valid()) {
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
                auto pod = ParseItemPOD(table.value());
                if (!pod) {
                    LOGW(name, " config invalid");
                    continue;
                }

                // assign POD to real struct
                for (const auto& operation : pod.value().operations) {
                    info.operations.push_back({operation, true});
                }

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