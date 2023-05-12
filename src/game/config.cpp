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

RaceProfConfig::RaceProfConfig(LuaManager& mgr, const RaceProfDef& defs, const std::string& filename) {
    auto lua = mgr.CreateSolitary(filename);
    auto root = lua.lua.get<std::optional<sol::table>>("Config");
    if (root) {
        auto data = ParseRacePOD(root.value());
        if (data) {
            data.value().professions.resize(defs.GetProfessions().size());
            // special treatment `professions` member
            auto profTable = root.value().get<std::optional<sol::table>>("professions");
            if (profTable) {
                for (int i = 0; i < defs.GetProfessions().size(); i++) {
                    auto& prof = defs.GetProfessions()[i];
                    auto table = profTable.value().get<sol::table>(prof);
                    if (table) {
                        auto prop = ParseMonsterPropertyRange(table);
                        if (prop) {
                            data.value().professions[i] = prop.value();
                        } else {
    // FIXME: too many else check
                            valid_ = false;
                        }
                    } else {
                        LOGE("profession ", prof, " not defined");
                        valid_ = false;
                    }
                }
                data_ = data.value();
            } else {
                valid_ = false;
            }
        } else {
            valid_ = false;
        }
    } else {
        valid_ = false;
    }
}

GameConfig::GameConfig(const std::string& configDir) {
    LuaManager mgr;
    raceProfDef_ = std::make_unique<RaceProfDef>(mgr, configDir + "definitions.lua");
    if (!raceProfDef_->Valid()) {
        LOGE("definition.lua config read failed!");
        valid_ = false;
    } else {
        for (const auto& race : raceProfDef_->GetRaces()) {
            std::string filename = configDir + race + ".lua";
            auto config = std::make_unique<RaceProfConfig>(mgr, *raceProfDef_, filename);
            if (!config->Valid()) {
                LOGE("read ", race , " config failed");
                valid_ = false;
                break;
            }
        }
    }
}
