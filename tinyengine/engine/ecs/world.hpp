#pragma once

#include "engine/ecs/component.hpp"
#include "engine/ecs/system.hpp"
#include "engine/ecs/entity.hpp"
#include "engine/core/event.hpp"

namespace engine {

class World final {
public:
    static World* Instance();
    static void Init();
    static void Quit();

    World();
    ~World();
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    Entity* CreateEntity(const std::string& name);

    template <typename... Args>
    Entity* CreateEntity(const std::string& name);

    void DestroyEntity(unsigned int id);
    void DestroyEntity(Entity* entity);

    template <typename T>
    T* CreateComponent();

    std::vector<std::unique_ptr<Entity>>& Entities() { return entities_; }
    const std::vector<std::unique_ptr<Entity>>& Entities() const { return entities_; }

    template <typename T>
    void RemoveComponent(T*);

    void RemoveComponent(Component*);

    void TryInitEntities();
    void Update();
    void CleanUp();

    template <typename T, typename... Args>
    System* AddSystem(Args&&... args);

    void AddSystem(std::unique_ptr<System>&& system) {
        systems_.push_back(std::move(system));
        system = nullptr;
    }

    void RemoveSystem(System* system);

    const std::vector<std::unique_ptr<System>>& GetSystems() const { return systems_; }

    void Shutdown();

private:
    unsigned int entityID_ = 0; 
    ComponentID componentID_ = 0; 

    std::vector<std::unique_ptr<Entity>> entities_;
    std::stack<std::unique_ptr<Entity>> entityTrashes_;
    std::vector<std::unique_ptr<System>> systems_;

    struct ComponentCell {
        std::vector<std::unique_ptr<Component>> components;
        std::stack<std::unique_ptr<Component>> componentTrashes_;
    };
    std::unordered_map<unsigned int, ComponentCell> components_;

    static std::unique_ptr<World> instance_;

    void destroyEntity(const std::vector<std::unique_ptr<Entity>>::const_iterator& it);
    void updateSystems(Entity*);
    void callEntityScripts(Entity*);

    void walkThroughNodeTree(engine::Entity* enitty, std::function<void(Entity*)> func);

    void initEntity(Entity*);
    void dispatchEvent2Entity(Entity*);
    template <typename T, typename... Args>
    void doCreateEntity(Entity* entity);

    void removeSystem(System* system);
};

template <typename T>
T* World::CreateComponent() {
    unsigned int id = ComponentIDHelper::GetID<T>();
    if (components_.find(id) == components_.end()) {
        components_[id] = ComponentCell{};
    }

    if (components_[id].componentTrashes_.empty()) {
        components_[id].components.emplace_back(std::make_unique<T>(componentID_++));
        return (T*)components_[id].components.back().get();
    } else {
        std::unique_ptr<Component> component = std::move(components_[id].componentTrashes_.top());
        component->id_ = componentID_++;
        components_[id].components.push_back(std::move(component));
        components_[id].componentTrashes_.pop();
        return (T*)components_[id].components.back().get();
    }
}

template <typename T>
void World::RemoveComponent(T* component) {
    if (!component) return;

    unsigned int id = ComponentIDHelper::GetID<T>();
    auto it = components_[id].components.begin();
    while (it != components_[id].components.end() && (*it)->ID() != component->ID()) {
        it++;
    }
    if (it != components_[id].components.end()) {
        auto& component = *it;
        component->Reset();
        components_[id].componentTrashes_.push(std::move(*it));
        components_[id].components.erase(it);
    }
}

template <typename T, typename... Args>
System* World::AddSystem(Args&&... args) {
    systems_.push_back(std::make_unique<T>(this, std::forward<Args>(args)...));
    return systems_.back().get();
}

template <typename T, typename... Args>
void World::doCreateEntity(Entity* entity) {
    entity->SetComponent(CreateComponent<T>());
    if constexpr (sizeof...(Args) != 0) {
        doCreateEntity<Args...>(entity);
    }
}

template <typename... Args>
Entity* World::CreateEntity(const std::string& name) {
    Entity* entity = CreateEntity(name);
    doCreateEntity<Args...>(entity);
    return entity;
}

}
