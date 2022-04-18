#pragma once

#include "pch.hpp"
#include "log.hpp"

namespace grogue::core {

class Scence {
public:
    /* use `string_view` means it won't change the string and hope you pass string-literal*/
    Scence(std::string_view name);

    virtual ~Scence() = default;

    virtual void OnInit() {}
    virtual void OnUpdate(uint32_t ms) {}
    virtual void OnRender() {}
    virtual void OnQuit() {}
    virtual void OnEventHandle(const SDL_Event&) {}

    std::string_view Name() const { return name_; }

private:
    std::string_view name_;
};


class ScenceMgr final {
public:
    static void Init(Scence*);
    static void Quit();

    template <typename T, typename... Args>
    static Scence* CreateScence(std::string_view name, Args&&... args) {
        auto it = scences_.find(name);
        if (it != scences_.end()) {
            LOG_ERROR("Scence {} exists", name);
            return nullptr;
        } else {
            Scence* scence = new T(name, std::forward<Args>(args)...);
            scences_[name] = std::unique_ptr<Scence>(scence);
            return scence;
        }
    }

    static void SwitchScence(std::string_view name);
    static Scence* GetScence(std::string_view name);
    static Scence* GetCurrentScence();
    static void CleanUpOldScence();

private:
    static std::unordered_map<std::string_view, std::unique_ptr<Scence>> scences_;
    static Scence* curScence_;
    static Scence* oldScence_;
};

}
