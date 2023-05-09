#include "game/config.hpp"

std::unique_ptr<GameConfig> GameConfig::instance_ = nullptr;

std::ostream& operator<<(std::ostream& o, const ConfigParseError& e) {
    o << "[ConfigParseError]"
      << "[" << ParseError::Type2Str(e.toml.type)
      << "]";
    switch (e.type) {
        case ParseErrorType::IOError:
            o << "[IOError]: " << e.io.desc;
            break;
        case ParseErrorType::ConstructEmpty:
            o << "construct invalid config object";
            break;
        case ParseErrorType::ParseError:
            o << "[ParseError]: ";
            for (auto node : e.toml.nodePath) {
                o << "->" << node;
            }
            o << std::endl << e.toml.desc;
            break;
    }
    return o;
}

BasicDefinitionConfig BasicDefinitionConfig::Load(const std::string& filename) {
    return BasicDefinitionConfig(filename);
}

#define BEGIN_PARSE() do {              
#define END_PARSE() } while (0)
#define ENSURE_FIELD_EXISTS(node, table, type, lambda) \
    if (!parseNode<type>(pathTracer_, node, table, lambda)) break;

BasicDefinitionConfig::BasicDefinitionConfig(const std::string& filename) {
    auto content = ReadWholeFile(filename);
    if (!content.has_value()) {
        error_.SetError(IOError{filename + " can't open"});
    } else if (toml::parse_result result = toml::parse(content.value());
               result) {
        const toml::table& table = result.table();
        BEGIN_PARSE() {
            ENSURE_FIELD_EXISTS("races", table, toml::array, [&](const toml::array& node) -> bool {
                node.for_each([&](auto&& elem) {
                    if constexpr (toml::is_string<decltype(elem)>) {
                        races_.push_back(*elem);
                    } else {
                        error_.SetError(ParseError{pathTracer_,
                            "exists non-string field",
                            ParseError::Type::TypeIncorrect,
                    });
                        return false;
                    }
                });
                return true;
            });

            ENSURE_FIELD_EXISTS(
                "professions", table, toml::array, [&](const toml::array& professions) -> bool {
                    professions.for_each([&](auto&& elem) {
                        if constexpr (toml::is_string<decltype(elem)>) {
                            professions_.push_back(*elem);
                        } else {
							error_.SetError(ParseError{pathTracer_,
								"exists non-string field",
								ParseError::Type::TypeIncorrect,
						});
                            return false;
                        }
                    });
                    return true;
                });
        }
        END_PARSE();
    } else {
        error_.SetError(
            ParseError{{}, "parse failed, may has syntax error"});
    }
}

void GameConfig::Init(const std::string& configDir) {
    instance_ = std::unique_ptr<GameConfig>(new GameConfig(configDir));
}

GameConfig& GameConfig::Instance() {
    return *instance_;
}

GameConfig::GameConfig(const std::string& configDir) {
    auto basic = BasicDefinitionConfig::Load(configDir + "/definitions.toml");
    defReader_ = std::make_unique<BasicDefinitionConfig>(basic);
    if (!basic.Failed()) {
        LOGI("read definitions config OK");
        defaultRaceProf_ = std::make_unique<RaceProfConfig>(RaceProfConfig::Load(configDir + "/default.toml", *defReader_));
        if (!defaultRaceProf_->Failed()) {
            LOGI("read default race config OK");
        } else {
            LOGE("default.toml parse failed", defaultRaceProf_->GetError());
        }
    } else {
        LOGE("definition.toml parse failed", basic.GetError());
    }
}

RaceProfConfig RaceProfConfig::Load(const std::string& filename, const BasicDefinitionConfig& definition) {
    return RaceProfConfig(filename, definition);
}

RaceProfConfig::RaceProfConfig(const std::string& filename,
                               const BasicDefinitionConfig& definition) {
    profAddition_.resize(definition.GetProfCount());

    auto content = ReadWholeFile(filename);
    if (!content.has_value()) {
        error_.SetError(IOError{filename + " can't open"});
    } else if (toml::parse_result result = toml::parse(content.value());
               result) {
        BEGIN_PARSE() {
            const toml::table& table = result.table();
            ENSURE_FIELD_EXISTS("race", table, toml::value<std::string>, [&](const toml::value<std::string>& race) -> bool {
				if (race.get() != "default") {
                        error_.SetError(
                            ParseError{pathTracer_, "default race config field `race` must be \"default\"", ParseError::Type::Custom});
					return false;
				}
				raceId_ = -1;
                return true;
            });

            ENSURE_FIELD_EXISTS("basic", table, toml::table, [&](const toml::table& basic) -> bool {
                if (auto basicProp =
                        parseMonsterProperty(filename, basic);
                    basicProp.has_value()) {
                    basic_ = std::move(basicProp.value());
                    return true;
                }
				return false;
            });

            ENSURE_FIELD_EXISTS("max", table, toml::table, [&](const toml::table& max) -> bool {
                if (auto maxProp =
                        parseMonsterProperty(filename, max);
                    maxProp.has_value()) {
                    max_ = std::move(maxProp.value());
                    return true;
                }
				return false;
            });
        }
        END_PARSE();
    } else {
        error_.SetError(ParseError{pathTracer_,
                                   "parse failed, may has syntax error",
                                   ParseError::Type::SyntaxError});
    }
}

std::optional<MonsterProperty> RaceProfConfig::parseMonsterProperty(
    const std::string& filename, const toml::table& table) {
    MonsterProperty prop;
    do {
        ENSURE_FIELD_EXISTS("hp", table, toml::value<int64_t>, [&](const toml::value<int64_t>& hp) -> bool {
            prop.hp = hp.get();
            return true;
        });
        ENSURE_FIELD_EXISTS("mp", table, toml::value<int64_t>, [&](const toml::value<int64_t>& mp) -> bool {
            prop.mp = mp.get();
            return true;
        })
        ENSURE_FIELD_EXISTS("strength", table, toml::value<int64_t>, [&](const toml::value<int64_t>& strength) -> bool {
            prop.ability.strength = strength.get();
            return true;
        })
        ENSURE_FIELD_EXISTS(
            "intelligence", table, toml::value<int64_t>, [&](const toml::value<int64_t>& intelligence) -> bool {
                prop.ability.intelligence = intelligence.get();
                return true;
            })
        ENSURE_FIELD_EXISTS("outsight", table, toml::value<int64_t>, [&](const toml::value<int64_t>& outsight) -> bool {
            prop.ability.outsight = outsight.get();
            return true;
        })
        ENSURE_FIELD_EXISTS(
            "constitution", table, toml::value<int64_t>, [&](const toml::value<int64_t>& constitution) -> bool {
                prop.ability.constitution = constitution.as_integer()->get();
                return true;
            })
        ENSURE_FIELD_EXISTS("agility", table, toml::value<int64_t>, [&](const toml::value<int64_t>& agility) -> bool {
            prop.ability.agility = agility.get();
            return true;
        })
        ENSURE_FIELD_EXISTS("nutrition", table, toml::value<int64_t>, [&](const toml::value<int64_t>& nutrition) -> bool {
            prop.ability.nutrition = nutrition.get();
            return true;
        })

        return prop;
    } while (0);
    return std::nullopt;
}
