#pragma once

#include "core/pch.hpp"
#include "game/types.hpp"

enum class ParseErrorType {
    NoError,
    IOError,
    ParseError,
    ConstructEmpty,
};

struct IOError final {
    std::string desc;
};

struct ParseError final {
    std::vector<std::string_view>
        nodePath;  // parse stack contain node path in config file
    std::string desc;

    enum class Type {
        Unknown,
        FieldNotFound,
        SyntaxError,
        TypeIncorrect,
        Custom,
    } type = Type::Unknown;

    static std::string_view Type2Str(Type type) {
#define CASE(e)   \
    case Type::e: \
        return #e;
        switch (type) {
            CASE(Unknown);
            CASE(FieldNotFound);
            CASE(SyntaxError);
            CASE(TypeIncorrect);
            CASE(Custom);
            default:
                return "Unknwon";
        }
#undef CASE
    }
};

struct ConfigParseError final {
    ParseErrorType type;
    IOError io;
    ParseError toml;

    ConfigParseError() : type(ParseErrorType::NoError) {}
    ConfigParseError(IOError e) : type(ParseErrorType::IOError), io(e) {}
    ConfigParseError(ParseError e)
        : type(ParseErrorType::ParseError), toml(e) {}

    ~ConfigParseError() {}

    void SetError(IOError e) {
        type = ParseErrorType::IOError;
        io = e;
    }

    void SetError(ParseError e) {
        type = ParseErrorType::ParseError;
        toml = e;
    }

    ConfigParseError& operator=(const ConfigParseError& o) = default;
    ConfigParseError(const ConfigParseError&) = default;
    ConfigParseError(ConfigParseError&&) = default;
};

std::ostream& operator<<(std::ostream& o, const ConfigParseError& e);

class ConfigReader {
public:
    using ErrorHandler = std::function<bool(ParseError::Type)>;
    static ErrorHandler DefaultErrorHandler;

    ConfigReader() = default;
    ConfigReader(const ConfigReader& o) { error_ = o.error_; }
    ConfigReader(ConfigReader&&) = default;

    //! Whether config file parse failed
    bool Failed() const { return error_.type != ParseErrorType::NoError; }
    //! If Failed() return false, you can get error here
    const ConfigParseError& GetError() const { return error_; }
    virtual ~ConfigReader(){};

protected:
    ConfigParseError error_;  // error state
    std::vector<std::string_view> pathTracer_;

    //! help function to get node safely(auto push non-exists node into error queue
    template <typename T>
    bool parseNode(std::vector<std::string_view>& pathTracer,
                   std::string_view name, const toml::table& table,
                   std::function<bool(const T& elem)> deal, 
                   ErrorHandler handler = ConfigReader::DefaultErrorHandler) {
        pathTracer.push_back(name);
        if (toml::node_view node = table[name]; !node) {
            auto errType = ParseError::Type::FieldNotFound;
            if (handler(errType)) {
                return true;
            } else {
                error_.SetError(ParseError{
                    pathTracer,
                    "field " + std::string(name) + " not found",
                    errType,
                });
                return false;
            }
        } else {
            if (auto elem = node.as<T>(); elem) {
                if (deal(*elem)) {
                    pathTracer.pop_back();
                    return true;
                }
                if (error_.type == ParseErrorType::NoError) {
                    auto errType = ParseError::Type::Unknown;
                    if (handler(errType)) {
                        return true;
                    } else {
                        error_.SetError(ParseError{
                            pathTracer, "unkown error", errType,
                        });
                        return false;
                    }
                }
                return false;
            } else {
                auto type =  ParseError::Type::TypeIncorrect;
                if (handler(type)) {
                    return true;
                } else {
                    error_.SetError(ParseError{
                        pathTracer, "field " + std::string(name) + " type is not satisfied",
                    });
                    return false;
                }
            }
        }
    }
};

class BasicDefinitionConfig final : public ConfigReader {
public:
    static BasicDefinitionConfig Load(const std::string& filename);

    using Container = std::vector<std::string>;

    BasicDefinitionConfig(ConfigParseError err) { error_ = err; }

    std::optional<size_t> GetRaceID(std::string_view name) const {
        return getIDFrom(name, races_);
    }
    size_t GetRaceCount() const { return races_.size(); }
    size_t GetProfCount() const { return professions_.size(); }
    std::optional<size_t> GetProfID(std::string_view name) const {
        return getIDFrom(name, professions_);
    }
    const Container& GetRaceNames() const { return races_; }
    const Container& GetProfessionNames() const { return professions_; }

private:
    Container races_;
    Container professions_;

    BasicDefinitionConfig(const std::string& content);

    std::optional<size_t> getIDFrom(std::string_view name,
                                    const Container& cont) const {
        if (auto it = std::find(cont.begin(), cont.end(), name);
            it != cont.end()) {
            return (it - cont.begin());
        } else {
            return std::nullopt;
        }
    }
};

class RaceProfConfig : public ConfigReader {
public:
    static RaceProfConfig Load(const std::string& filename,
                               const BasicDefinitionConfig& definition);
    static RaceProfConfig LoadByDefault(const std::string& configDir,
                                        const std::string& raceName,
                                        const BasicDefinitionConfig& definition,
                                        RaceProfConfig& defaultConfig);

    const MonsterProperty& GetBasic() const { return basic_; }
    const MonsterProperty& GetMax() const { return max_; }
    const std::vector<MonsterProperty>& GetProfAdditions() const {
        return profAddition_;
    }
    const std::string& GetName() const { return name_; }
    int GetRaceID() const { return raceId_; }

private:
    MonsterProperty basic_;                      // basic property
    MonsterProperty max_;                        // max property
    std::vector<MonsterProperty> profAddition_;  // professions addition
    std::string name_;
    int raceId_ =
        -1;  // race ID(race index in BasicDefinitionConfig), -1 means "default"

    RaceProfConfig(const std::string& filename,
                   const BasicDefinitionConfig& definition);
    RaceProfConfig(const std::string& configDir,
                   const std::string& filename,
                   const BasicDefinitionConfig& definition,
                   RaceProfConfig& defaultConfig);
    std::optional<MonsterProperty> parseMonsterProperty(const toml::table& table);
    std::optional<MonsterProperty> parseMonsterPropertyByDefault(const toml::table& table, const MonsterProperty& defaultProp);

    bool parseAllProperty(const toml::table& table);
    bool parseAllPropertyWithDefault(const std::string& name, const toml::table& table, const BasicDefinitionConfig& defs, const RaceProfConfig& defaultConfig);
};

struct GameConfig final {
public:
    static void Init(const std::string& configDir);
    static GameConfig& Instance();

    const BasicDefinitionConfig& GetBasicDefConfig() const {
        if (!defReader_) {
            throw std::runtime_error("basic definition config incorrect");
        }
        return *defReader_;
    }

private:
    static std::unique_ptr<GameConfig> instance_;

    std::unique_ptr<RaceProfConfig> defaultRaceProf_;
    std::vector<RaceProfConfig> raceConfigs_;
    std::unique_ptr<BasicDefinitionConfig> defReader_;

    GameConfig(const std::string& configDir);
};