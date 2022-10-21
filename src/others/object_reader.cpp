#include "object_reader.hpp"

static std::array<std::string_view, ObjectConfig::TypeNum> TypeMap{
    "unknown",
    "block",
    "pickup",
};

ObjectConfig::Type ObjectConfig::GetTypeFromStr(std::string_view str) {
    for (int i = 0; i < TypeMap.size(); i++) {
        if (TypeMap[i] == str) {
            return static_cast<ObjectConfig::Type>(i);
        }
    }
    return ObjectConfig::Type::Unknown;
}

bool ObjectConfig::isFeatureHasProperty(std::string_view feature) {
    return feature.find("=") != feature.npos;
}

std::pair<std::string_view, int> ObjectConfig::parseFeature(std::string_view feature) {
    std::pair<std::string_view, int> result;
    auto delimIdx = feature.find("=");
    result.first = feature.substr(delimIdx);
    result.second = atoi(feature.substr(delimIdx + 1).data());
    return result;
}

#define TRY_SET_FEATURE(x) if (prop.first == #x) this->feature.x = prop.second;

void ObjectConfig::TurnOnFeature(const std::string_view feature) {
    if (isFeatureHasProperty(feature)) {
        auto prop = parseFeature(feature);
        TRY_SET_FEATURE(beat);
        TRY_SET_FEATURE(cut);
        TRY_SET_FEATURE(beCutted);
        TRY_SET_FEATURE(beBeated);
    } else {
    }
}

#undef TRY_SET_FEATURE


ObjectConfig ObjectConfigReader::Read(const std::filesystem::path& filepath) {
    ObjectConfig config;
    auto tbl = toml::parse_file(filepath.c_str());
    config.id = tbl["id"].value<int>().value();
    config.name = tbl["name"].value<std::string>().value_or("[No Name]");
    auto imgName = tbl["img"].value<std::string>().value();
    if (!engine::ImageFactory::Find(imgName, config.image)) {
        Loge("object image {} not found", imgName);
    }
    config.description = tbl["description"].value<std::string>().value_or("");
    config.type = ObjectConfig::GetTypeFromStr(tbl["type"].value<std::string>().value());
    auto features = tbl["features"].as_array();
    if (features) {
        for (auto& feature : *features) {
            config.TurnOnFeature(feature.value<std::string_view>().value());
        }
    }
    auto drop = tbl["drop"].as_array();
    if (drop) {
        for (auto& elem : *drop) {
            auto array = elem.as_table();
            ObjectFeature::DropObject dropObject;
            dropObject.name = array->at("name").value<std::string>().value();
            dropObject.num = array->at("num").value<int>().value();
            config.feature.dropObjects.push_back(dropObject);
        }
    }
    return config;
}

std::unordered_map<std::string, ObjectID> ObjectConfigStorage::idNameMap;
std::unordered_map<ObjectID, ObjectConfig> ObjectConfigStorage::configs;

void ObjectConfigStorage::LoadAllConfig(const std::filesystem::path& root) {
    if (!std::filesystem::exists(root)) {
        Loge("load config failed: root dir not exists - {}", root);
        return;
    }
    
    loadConfigRecursive(root, root);
}

void ObjectConfigStorage::loadConfigRecursive(const std::filesystem::path& root, const std::filesystem::path& path) {
    if (std::filesystem::is_directory(path)) {
        std::filesystem::directory_iterator dirIter(path);
        while (dirIter != std::filesystem::directory_iterator()) {
            loadConfigRecursive(root, *dirIter);
            dirIter++;
        }
    } else {
        ObjectConfigReader reader;
        auto config = reader.Read(path);
        configs[config.id] = config;
        auto name = RemoveRootFromPath(root.string(), GetFilenameNoExt(path.string()));
        CvtWindowsDelim2Unix(name);
        idNameMap[name] = config.id;
    }
}

bool ObjectConfigStorage::Find(ObjectID id, ObjectConfig& config) {
    auto it = configs.find(id);
    if (it == configs.end()) {
        return false;
    }
    config = it->second;
    return true;
}

bool ObjectConfigStorage::Find(const std::string& name, ObjectConfig& config) {
    auto it = idNameMap.find(name);
    if (it == idNameMap.end()) return false;
    return Find(it->second, config);
}