#include "app/animation.hpp"
#include "app_animation_refl.hpp"

AnimatedClip<float> parseAnimFromArray(const sol::table& table) {
    std::vector<Frame<float>> frames;

    for (const auto& frame : table) {
        if (!table) {
            return nullptr;
        }

        auto config = serialize::DeserializeFromLua<FrameConfig>(frame.second.as<sol::table>());
        InterpFunc<float> func;
        if (config.value().interpolation == "linear") {
            func = math::Lerp<float>;
        } else if (config.value().interpolation == "none") {
            func = NoInterpolation<float>;
        }
        frames.push_back(CreateBasicPropFrame<float>(
            config.value().value, config.value().time, func));
    }

    std::sort(frames.begin(), frames.end(),
              [](const Frame<float> f1, const Frame<float>& f2) {
                  return f1.Duration() < f2.Duration();
              });

    auto clip = CreateAnimClip<float>();
    // FIXME use std::move to improve
    for (auto& frame : frames) {
        clip->AppendFrame(frame);
    }

    return clip;
}

AnimatedClip<float> LoadAnim(LuaManager& manager, const std::string& filename) {
    auto lua = manager.CreateSolitary(filename);
    auto table = lua.lua.get<std::optional<sol::table>>("Config");
    if (!table) return nullptr;

    return parseAnimFromArray(table.value());
}

std::unordered_map<std::string, AnimatedClip<float>> LoadAnims(
    LuaManager& manager, const std::string& filename) {
    auto lua = manager.CreateSolitary(filename);
    std::unordered_map<std::string, AnimatedClip<float>> results;

    auto table = lua.lua.get<std::optional<sol::table>>("Config");

    for (const auto& clip : table.value()) {
        std::string name = clip.first.as<std::string>();
        auto arr = clip.second.as<sol::table>();

        results[name] = parseAnimFromArray(arr);
    }

    return results;
}
