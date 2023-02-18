#include "app/animation.hpp"

AnimatedClip<float> loadAnimFromTable(sol::table& frames) {
    auto clip = CreateAnimClip<float>();

    std::vector<Frame<float>> finalFrames;

    for (auto& frame : frames) {
        auto frameConfigTable = frame.second.as<sol::table>();
        uint32_t time = frameConfigTable.get<uint32_t>("time");
        float value = frameConfigTable.get<float>("value");
        std::string interpolation = frameConfigTable.get<std::string>("interpolation");

        InterpFunc<float> func;
        if (interpolation == "linear") {
            func = math::Lerp<float>;
        } else if (interpolation == "none") {
            func = NoInterpolation<float>;
        }

        finalFrames.push_back(CreateBasicPropFrame(value, time, func));
    }

    std::sort(finalFrames.begin(), finalFrames.end(), [](const Frame<float> f1, const Frame<float>& f2) {
        return f1.Duration() < f2.Duration();
    });

    // FIXME use std::move to improve
    for (auto& frame : finalFrames) {
        clip->AppendFrame(frame);
    }

    return clip;

}

AnimatedClip<float> LoadAnim(LuaManager& manager, const std::string& filename) {
    auto lua = manager.CreateSolitary(filename);
    auto table = lua.lua.get<sol::table>("frames");

    return loadAnimFromTable(table);
}

std::unordered_map<std::string, AnimatedClip<float>> LoadAnims(LuaManager& manager, const std::string& filename) {
    auto lua = manager.CreateSolitary(filename);
    std::unordered_map<std::string, AnimatedClip<float>> results;

    auto table = lua.lua.get<sol::table>("clips");

    for (auto& clipTable : table) {
        std::string name = clipTable.first.as<std::string>();
        sol::table clipContent = clipTable.second.as<sol::table>();
        auto clip = loadAnimFromTable(clipContent);

        results[name] = clip;
    }

    return results;
}