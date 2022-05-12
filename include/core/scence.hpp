#pragma once

#include "pch.hpp"
#include "log.hpp"
#include "layer.hpp"
#include "event_dispatcher.hpp"
#include "storage.hpp"
#include "timer.hpp"
#include "animation.hpp"

namespace grogue::core {

class Scence {
public:
    friend class ScenceMgr;

    /* use `string_view` means it won't change the string and hope you pass string-literal*/
    Scence(std::string_view name);

    virtual ~Scence() = default;

    virtual void OnInit() {}
    virtual bool OnQuit() { return true; }

    void OnUpdate();
    void OnRender();
    void OnEventHandle(const SDL_Event&);

    std::string_view Name() const { return name_; }

    template <typename LayerT, typename... Args>
    void PushBackLayer(std::string_view name, Args... args) {
        layers_.emplace_back(new LayerT(name, std::forward<Args>(args)...));
        layers_.back()->scence_ = this;
    }

    template <typename LayerT, typename... Args>
    void PushFrontLayer(std::string_view name, Args... args) {
        layers_.emplace_front(new LayerT(name, std::forward<Args>(args)...));
        layers_.front()->scence_ = this;
    }

    Layer* FindLayer(std::string_view name) const {
        auto it = std::find_if(layers_.begin(), layers_.end(),
                               [=](const std::unique_ptr<Layer>& layer) {
                                   return layer->GetName() == name;
                               });
        if (it != layers_.end()) {
            return nullptr;
        } else {
            return it->get();
        }
    }

    AnimationMgr& GetAnimMgr() { return animManager_; }

private:
    std::string_view name_;
    std::list<std::unique_ptr<Layer>> layers_;
    AnimationMgr animManager_;
};


class ScenceMgr final {
public:
    static void Init(Scence*);
    static void Quit();

    template <typename T, typename... Args>
    static Scence* CreateScence(std::string_view name, Args&&... args) {
        if (storage_.IsExists(name)) {
            LOG_ERROR("Scence {} exists", name);
            return nullptr;
        } else {
            Scence* scence = new T(name, std::forward<Args>(args)...);
            storage_.Create(name, std::forward<Args>(args)...);
            return scence;
        }
    }

    static void SwitchScence(std::string_view name);
    static Scence* GetScence(std::string_view name);
    static Scence* GetCurrentScence();
    static void CleanUpOldScence();

private:
    static Storage<std::string_view, std::unique_ptr<Scence>> storage_;
    static Scence* curScence_;
    static Scence* oldScence_;
};

}
