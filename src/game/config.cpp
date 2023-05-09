#include "game/config.hpp"

std::unique_ptr<GameConfig> GameConfig::instance_ = nullptr;
ConfigReader::ErrorHandler ConfigReader::DefaultErrorHandler = [](ParseError::Type){ return false; };

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
#define ENSURE_FIELD_EXISTS(node, table, type, lambda, ...) if (!parseNode<type>(pathTracer_, node, table, lambda, ##__VA_ARGS__)) break;

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
        parseAllProperty(result.table());
    } else {
        error_.SetError(ParseError{pathTracer_,
                                   "parse failed, may has syntax error",
                                   ParseError::Type::SyntaxError});
    }
}

bool RaceProfConfig::parseAllProperty(const toml::table& table) {
    BEGIN_PARSE() {
        ENSURE_FIELD_EXISTS("name", table, toml::value<std::string>, [&](const toml::value<std::string>& name) -> bool {
            raceId_ = -1;
            name_ = name.get();
            return true;
        });

        ENSURE_FIELD_EXISTS("basic", table, toml::table, [&](const toml::table& basic) -> bool {
            if (auto basicProp =
                    parseMonsterProperty(basic);
                basicProp.has_value()) {
                basic_ = std::move(basicProp.value());
                return true;
            }
            return false;
        });

        ENSURE_FIELD_EXISTS("max", table, toml::table, [&](const toml::table& max) -> bool {
            if (auto maxProp =
                    parseMonsterProperty(max);
                maxProp.has_value()) {
                max_ = std::move(maxProp.value());
                return true;
            }
            return false;
        });
    }
    END_PARSE();
    return true;
}


std::optional<MonsterProperty> RaceProfConfig::parseMonsterProperty(const toml::table& table) {
    MonsterProperty prop;
    do {
        ENSURE_FIELD_EXISTS("hp", table, toml::value<int64_t>, [&](const toml::value<int64_t>& hp) -> bool {
            prop.hp = hp.get();
            return true;
        });
        ENSURE_FIELD_EXISTS("mp", table, toml::value<int64_t>, [&](const toml::value<int64_t>& mp) -> bool {
            prop.mp = mp.get();
            return true;
        });
        ENSURE_FIELD_EXISTS("strength", table, toml::value<int64_t>, [&](const toml::value<int64_t>& strength) -> bool {
            prop.strength = strength.get();
            return true;
        });
        ENSURE_FIELD_EXISTS(
            "intelligence", table, toml::value<int64_t>, [&](const toml::value<int64_t>& intelligence) -> bool {
                prop.intelligence = intelligence.get();
                return true;
            });
        ENSURE_FIELD_EXISTS("outsight", table, toml::value<int64_t>, [&](const toml::value<int64_t>& outsight) -> bool {
            prop.outsight = outsight.get();
            return true;
        });
        ENSURE_FIELD_EXISTS(
            "constitution", table, toml::value<int64_t>, [&](const toml::value<int64_t>& constitution) -> bool {
                prop.constitution = constitution.as_integer()->get();
                return true;
            });
        ENSURE_FIELD_EXISTS("agility", table, toml::value<int64_t>, [&](const toml::value<int64_t>& agility) -> bool {
            prop.agility = agility.get();
            return true;
        });
        ENSURE_FIELD_EXISTS("nutrition", table, toml::value<int64_t>, [&](const toml::value<int64_t>& nutrition) -> bool {
            prop.nutrition = nutrition.get();
            return true;
        });

        return prop;
    } while (0);
    return std::nullopt;
}

bool RaceProfConfig::parseAllPropertyWithDefault(const std::string& raceName, const toml::table& table, const BasicDefinitionConfig& defs, const RaceProfConfig& defaultConfig) {
    BEGIN_PARSE() {
        ENSURE_FIELD_EXISTS("name", table, toml::value<std::string>, [&](const toml::value<std::string>& name) -> bool {
            name_ = name.get();
            if (auto id = defs.GetRaceID(raceName); id.has_value()) {
                raceId_ = id.value();
                return true;
            } else {
                error_.SetError(ParseError{pathTracer_, "race " + name_ + " not fount, may not define in definitions.toml?", ParseError::Type::Custom});
                return false;
            }
        });

        ENSURE_FIELD_EXISTS("basic", table, toml::table, [&](const toml::table& basic) -> bool {
            if (auto basicProp =
                    parseMonsterPropertyByDefault(basic, defaultConfig.GetBasic());
                basicProp.has_value()) {
                basic_ = std::move(basicProp.value());
                return true;
            }
            return false;
        });

        ENSURE_FIELD_EXISTS("max", table, toml::table, [&](const toml::table& max) -> bool {
            if (auto maxProp =
                    parseMonsterPropertyByDefault(max, defaultConfig.GetMax());
                maxProp.has_value()) {
                max_ = std::move(maxProp.value());
                return true;
            }
            return false;
        });
    }
    END_PARSE();
    return true;
}

RaceProfConfig RaceProfConfig::LoadByDefault(const std::string& configDir, const std::string& raceName,
                                        const BasicDefinitionConfig& definition,
                                        RaceProfConfig& defaultProp) {
    return RaceProfConfig(configDir, raceName, definition, defaultProp);
}

RaceProfConfig::RaceProfConfig(const std::string& configDir,
                               const std::string& raceName,
                               const BasicDefinitionConfig& definition,
                               RaceProfConfig& defaultConfig)
    : name_(raceName) {
    auto id = definition.GetRaceID(raceName);
    if (id.has_value()) {
        raceId_ = id.value();

        profAddition_.resize(definition.GetProfCount());

        std::string filename = configDir + raceName + ".toml";
        auto content = ReadWholeFile(filename);
        if (!content.has_value()) {
            error_.SetError(IOError{filename + " can't open"});
        } else if (toml::parse_result result = toml::parse(content.value()); result) {
            parseAllPropertyWithDefault(raceName, result.table(), definition, defaultConfig);
        } else {
            error_.SetError(ParseError{pathTracer_,
                                       "parse failed, may has syntax error",
                                       ParseError::Type::SyntaxError});
        }
    }
}

// use for RaceProfConfig::parseAllPropertyWithDefault()
template <typename T>
bool GenericErrorHandler(ParseError::Type type, const MonsterProperty& defaultProp, MonsterProperty& prop, T pointer) {
    if (type == ParseError::Type::FieldNotFound) {
        prop.*pointer = defaultProp.*pointer; 
        return true;
    }
    return false;
}

#define DEFAULT_FIELD_PROCESS(field) [&](ParseError::Type type) { return GenericErrorHandler(type, defaultProp, prop, &MonsterProperty::field); }

std::optional<MonsterProperty> RaceProfConfig::parseMonsterPropertyByDefault(const toml::table& table, const MonsterProperty& defaultProp) {
    MonsterProperty prop;
    do {
        ENSURE_FIELD_EXISTS(
            "hp", table, toml::value<int64_t>,
            [&](const toml::value<int64_t>& hp) -> bool {
                prop.hp = hp.get();
                return true;
            },
            DEFAULT_FIELD_PROCESS(hp));
        ENSURE_FIELD_EXISTS("mp", table, toml::value<int64_t>, [&](const toml::value<int64_t>& mp) -> bool {
            prop.mp = mp.get();
            return true;
        }, DEFAULT_FIELD_PROCESS(mp));
        ENSURE_FIELD_EXISTS("strength", table, toml::value<int64_t>, [&](const toml::value<int64_t>& strength) -> bool {
            prop.strength = strength.get();
            return true;
        }, DEFAULT_FIELD_PROCESS(strength));
        ENSURE_FIELD_EXISTS(
            "intelligence", table, toml::value<int64_t>, [&](const toml::value<int64_t>& intelligence) -> bool {
                prop.intelligence = intelligence.get();
                return true;
            }, DEFAULT_FIELD_PROCESS(intelligence));
        ENSURE_FIELD_EXISTS("outsight", table, toml::value<int64_t>, [&](const toml::value<int64_t>& outsight) -> bool {
            prop.outsight = outsight.get();
            return true;
        });
        ENSURE_FIELD_EXISTS(
            "constitution", table, toml::value<int64_t>, [&](const toml::value<int64_t>& constitution) -> bool {
                prop.constitution = constitution.as_integer()->get();
                return true;
            }, DEFAULT_FIELD_PROCESS(constitution));
        ENSURE_FIELD_EXISTS("agility", table, toml::value<int64_t>, [&](const toml::value<int64_t>& agility) -> bool {
            prop.agility = agility.get();
            return true;
        }, DEFAULT_FIELD_PROCESS(agility));
        ENSURE_FIELD_EXISTS("nutrition", table, toml::value<int64_t>, [&](const toml::value<int64_t>& nutrition) -> bool {
            prop.nutrition = nutrition.get();
            return true;
        }, DEFAULT_FIELD_PROCESS(nutrition));

        return prop;
    } while (0);
    return std::nullopt;
}
