#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "game/config.hpp"
#include "test_helper.hpp"

#define TEST_MONSTER_PROP(p) \
    
TEST_CASE("ReadDefinitionConfig") {
    BasicDefinitionConfig defConfig = BasicDefinitionConfig::Load(TestHelper::Instance().GetResourcePath() + "config/definitions.toml");
    REQUIRE_FALSE(defConfig.Failed());

    REQUIRE(defConfig.GetProfCount() == 1);
    REQUIRE(defConfig.GetRaceCount() == 1);

    std::vector<std::string> races = {"human"};
    REQUIRE(defConfig.GetRaceNames() == races);

    std::vector<std::string> professions = {"normal"};
    REQUIRE(defConfig.GetProfessionNames() == professions);

    RaceProfConfig defaultConfig = RaceProfConfig::Load(TestHelper::Instance().GetResourcePath() + "config/default.toml", defConfig);
    SECTION("[RaceProfConfig default test]") {
        RaceProfConfig& config = defaultConfig;
        REQUIRE_FALSE(config.Failed());
        auto& basic = config.GetBasic();
        REQUIRE(basic.hp == 10);
        REQUIRE(basic.mp == 10);
        REQUIRE(basic.strength == 10);
        REQUIRE(basic.intelligence == 10);
        REQUIRE(basic.outsight == 10);
        REQUIRE(basic.constitution == 10);
        REQUIRE(basic.agility == 10);
        REQUIRE(basic.nutrition == 1000);

        auto& max = config.GetMax();
        REQUIRE(max.hp == 100);
        REQUIRE(max.mp == 100);
        REQUIRE(max.strength == 100);
        REQUIRE(max.intelligence == 100);
        REQUIRE(max.outsight == 100);
        REQUIRE(max.constitution == 100);
        REQUIRE(max.agility == 100);
        REQUIRE(max.nutrition == 2000);

        REQUIRE(config.GetRaceID() == -1);
        REQUIRE(config.GetName() == "default");
    }

    SECTION("[RaceProfConfig test human]") {
        RaceProfConfig config = RaceProfConfig::LoadByDefault(TestHelper::Instance().GetResourcePath() + "config/", "human", defConfig, defaultConfig);
        REQUIRE_FALSE(config.Failed());
        auto& basic = config.GetBasic();
        REQUIRE(basic.hp == 10);
        REQUIRE(basic.mp == 10);
        REQUIRE(basic.strength == 10);
        REQUIRE(basic.intelligence == 10);
        REQUIRE(basic.outsight == 10);
        REQUIRE(basic.constitution == 10);
        REQUIRE(basic.agility == 10);
        REQUIRE(basic.nutrition == 1000);

        auto& max = config.GetMax();
        REQUIRE(max.hp == 100);
        REQUIRE(max.mp == 100);
        REQUIRE(max.strength == 100);
        REQUIRE(max.intelligence == 100);
        REQUIRE(max.outsight == 100);
        REQUIRE(max.constitution == 100);
        REQUIRE(max.agility == 100);
        REQUIRE(max.nutrition == 2000);

        REQUIRE(config.GetRaceID() == 0);
        REQUIRE(config.GetName() == "Human");
    }

    SECTION("[RaceProfConfig test lack fields]") {
        RaceProfConfig config = RaceProfConfig::LoadByDefault(TestHelper::Instance().GetResourcePath() + "config/", "human", defConfig, defaultConfig);
        REQUIRE_FALSE(config.Failed());
        auto& basic = config.GetBasic();
        REQUIRE(basic.hp == 10);
        REQUIRE(basic.mp == 10);
        REQUIRE(basic.strength == 10);
        REQUIRE(basic.intelligence == 10);
        REQUIRE(basic.outsight == 10);
        REQUIRE(basic.constitution == 10);
        REQUIRE(basic.agility == 10);
        REQUIRE(basic.nutrition == 1000);

        auto& max = config.GetMax();
        REQUIRE(max.hp == 100);
        REQUIRE(max.mp == 100);
        REQUIRE(max.strength == 100);
        REQUIRE(max.intelligence == 100);
        REQUIRE(max.outsight == 100);
        REQUIRE(max.constitution == 100);
        REQUIRE(max.agility == 100);
        REQUIRE(max.nutrition == 2000);

        REQUIRE(config.GetRaceID() == 0);
        REQUIRE(config.GetName() == "Lack");
    }
}