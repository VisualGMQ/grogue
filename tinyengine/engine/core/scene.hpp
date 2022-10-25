#pragma once

#include "engine/core/pch.hpp"
#include "engine/components/node.hpp"
#include "engine/ecs/entity.hpp"
#include "engine/core/init_config.hpp"

namespace engine {

class DLLEXPORT Scene {
public:
    friend class SceneMgr;

    Scene(const std::string& name);
    Scene(const Scene&) = delete;
    virtual ~Scene() = default;

    Scene& operator=(const Scene&) = delete;

    const std::string& Name() const { return name_; }
    Entity* GetRootEntity() const { return root_; }
    Entity* Get2DRoot() const { return node2d_; }
    Entity* GetUIRoot() const { return nodeUI_; }

    void Attach2D(Entity*);
    void Attach3D(Entity*);
    void AttachUI(Entity*);
    void Attach(Entity*);

    Entity* FindEntity(const std::string& name);
    std::vector<Entity*> FindEntities(const std::string& name);

    virtual void OnInit() {}
    virtual void OnQuit() {}

private:
    std::string name_;
    Entity* root_;
    Entity* node2d_;
    Entity* nodeUI_;

    void beforeInit();
    void beforeQuit();
};


class DLLEXPORT SceneMgr final {
public:
    static void Init();
    static void Quit();
    template <typename T>
    static void LoadScene(const std::string& name);
    static void ChangeScene(const std::string& name);
    static Scene* CurrentScene() { return curScene_; }
    static void QuitOldScene();

private:
    static Scene* curScene_;
    static Scene* oldScene_;
    static std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
};

template <typename T>
void SceneMgr::LoadScene(const std::string& name) {
    auto it = scenes_.find(name);
    if (it == scenes_.end()) {
        scenes_[name] = std::make_unique<T>(name);
    } else {
        Logw("scene {} already loaded", name.c_str());
    }
}

#define SCENE_CONFIG() void GameInit(void)
#define LOAD_SCENE(scene) engine::SceneMgr::LoadScene<scene>(#scene)
#define ENTER_SCENE(scene) engine::SceneMgr::ChangeScene(#scene)

}
