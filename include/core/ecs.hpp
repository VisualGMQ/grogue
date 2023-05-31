#pragma once
#include <algorithm>
#include <cassert>
#include <functional>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>

#include "sparse_sets.hpp"
#include "log.hpp"

#define assertm(msg, expr) assert(((void)msg, (expr)))

namespace ecs {  // fwd declare

using ComponentID = uint32_t;
using Entity = uint32_t;

}  // namespace ecs

//! @brief ECS Component, identify a node entity in herarchy
//! @note maybe you think component shouldn't in ecs.hpp,
//!        but for supporting herarchy in ecs, we must put it here(for
//!        HierarchyUpdateSystem)
struct Node final {
    std::optional<ecs::Entity>
        parent;  //!< parent node, std::nullopt means this node is root
    std::vector<ecs::Entity> children;
};

namespace ecs {

class IndexGetter final {
public:
    template <typename T>
    static uint32_t Get() {
        static uint32_t id = curIdx_++;
        return id;
    }

private:
    inline static uint32_t curIdx_ = 0;
};

template <typename T, typename = std::enable_if<std::is_integral_v<T>>>
struct IDGenerator final {
public:
    static T Gen() { return curId_++; }

private:
    inline static T curId_ = {};
};

template <typename T>
class EventStaging final {
public:
    static void Set(const T &t) { event_ = t; }

    static void Set(T &&t) { event_ = std::move(t); }

    static T &Get() { return *event_; }

    static bool Has() { return event_ != std::nullopt; }

    static void Clear() { event_ = std::nullopt; }

private:
    inline static std::optional<T> event_ = std::nullopt;
};

template <typename T>
class EventReader final {
public:
    bool Has() const { return EventStaging<T>::Has(); }

    const T &Read() { return EventStaging<T>::Get(); }

    void Clear() { EventStaging<T>::Clear(); }
};

class World;

class Events final {
public:
    friend class World;

    template <typename T>
    friend class EventWriter;

    template <typename T>
    auto Reader();

    template <typename T>
    auto Writer();

private:
    std::vector<void (*)(void)> removeEventFuncs_;
    std::vector<std::function<void(void)>> addEventFuncs_;

    void addAllEvents() {
        for (auto func : addEventFuncs_) {
            func();
        }
        addEventFuncs_.clear();
    }

    void removeAllEvents() {
        for (auto func : removeEventFuncs_) {
            func();
        }
        removeEventFuncs_.clear();
    }
};

template <typename T>
class EventWriter final {
public:
    EventWriter(Events &e) : events_(e) {}

    void Write(const T &t);

private:
    Events &events_;
};

template <typename T>
auto Events::Reader() {
    removeEventFuncs_.push_back([]() { EventStaging<T>::Clear(); });
    return EventReader<T>{};
}

template <typename T>
auto Events::Writer() {
    return EventWriter<T>{*this};
}

template <typename T>
void EventWriter<T>::Write(const T &t) {
    events_.addEventFuncs_.push_back([=]() { EventStaging<T>::Set(t); });
}

using EntityGenerator = IDGenerator<Entity>;

class Commands;
class Resources;
class Querier;

using EachElemUpdateSystem = void(*)(Commands &, Querier, Resources, Events &);
using HierarchyUpdateSystem = void(*)(std::optional<Entity>, Entity,
                                       Commands &, Querier, Resources,
                                       Events &);
using StartupSystem = void(*)(Commands &, Resources);

using UpdateSystem = std::variant<EachElemUpdateSystem, HierarchyUpdateSystem>;

class Plugins {
public:
    virtual ~Plugins() = default;

    virtual void Build(World *world) = 0;
    virtual void Quit(World *world) = 0;
};

class World final {
public:
    friend class Commands;
    friend class Resources;
    friend class Querier;
    friend class CondQuerier;
    using ComponentContainer = std::unordered_map<ComponentID, void *>;

    World() = default;
    World(const World &) = delete;
    World &operator=(const World &) = delete;

    World &AddStartupSystem(StartupSystem sys) {
        startupSystems_.push_back(sys);

        return *this;
    }

    World &AddSystem(UpdateSystem sys) {
        updateSystems_.push_back(sys);

        return *this;
    }

    template <typename T>
    World &SetResource(T &&resource);

    template <typename T>
    T *GetResource();

    template <typename T, typename... Args>
    World &AddPlugins(Args &&...args) {
        static_assert(std::is_base_of_v<Plugins, T>);
        pluginsList_.push_back(
            std::make_unique<T>(std::forward<Args>(args)...));
        return *this;
    }

    void Startup();
    void Update();

    void Shutdown() {
        entities_.clear();
        resources_.clear();
        componentMap_.clear();
        for (auto &plugin : pluginsList_) {
            plugin->Quit(this);
        }
    }

private:
    struct Pool final {
        std::vector<void *> instances;
        std::vector<void *> cache;

        using CreateFunc = void *(*)(void);
        using DestroyFunc = void (*)(void *);

        CreateFunc create;
        DestroyFunc destroy;

        Pool(CreateFunc create, DestroyFunc destroy)
            : create(create), destroy(destroy) {
            assertm("you must give a non-nullptr create func", create);
            assertm("you must give a non-nullptr destroy func", create);
        }

        void *Create() {
            if (!cache.empty()) {
                instances.push_back(cache.back());
                cache.pop_back();
            } else {
                instances.push_back(create());
            }
            return instances.back();
        }

        void Destroy(void *elem) {
            if (auto it = std::find(instances.begin(), instances.end(), elem);
                it != instances.end()) {
                cache.push_back(*it);
                std::swap(*it, instances.back());
                instances.pop_back();
            } else {
                assertm("your element not in pool", false);
            }
        }
    };

    struct ComponentInfo {
        Pool pool;
        SparseSets<Entity, 32> sparseSet;

        ComponentInfo(Pool::CreateFunc create, Pool::DestroyFunc destroy)
            : pool(create, destroy) {}

        ComponentInfo() : pool(nullptr, nullptr) {}
    };

    using ComponentMap = std::unordered_map<ComponentID, ComponentInfo>;
    ComponentMap componentMap_;
    std::unordered_map<Entity, ComponentContainer> entities_;
    std::vector<std::unique_ptr<Plugins>> pluginsList_;

    struct ResourceInfo {
        void *resource = nullptr;
        using DestroyFunc = void (*)(void *);
        DestroyFunc destroy = nullptr;

        ResourceInfo(DestroyFunc destroy) : destroy(destroy) {
            assertm("you must give a non-null destroy function", destroy);
        }

        // ResourceInfo() { int a = 123;
        // }
        ~ResourceInfo() { destroy(resource); }
    };

    std::unordered_map<ComponentID, ResourceInfo> resources_;
    std::vector<StartupSystem> startupSystems_;
    std::vector<UpdateSystem> updateSystems_;
};

class Commands final {
public:
    Commands(World &world) : world_(world) {}

    template <typename... ComponentTypes>
    Commands &Spawn(ComponentTypes &&...components) {
        SpawnAndReturn<ComponentTypes...>(
            std::forward<ComponentTypes>(components)...);
        return *this;
    }

    template <typename... ComponentTypes>
    Entity SpawnAndReturn(ComponentTypes &&...components) {
        EntitySpawnInfo info;
        info.entity = EntityGenerator::Gen();
        doSpawn<ComponentTypes...>(info.components,
                                   std::forward<ComponentTypes>(components)...);
        spawnEntities_.push_back(info);
        return info.entity;
    }

    template <typename... ComponentTypes>
    Commands &AddComponent(Entity entity, ComponentTypes &&...components) {
        EntitySpawnInfo info;
        info.entity = entity;
        doSpawn<ComponentTypes...>(info.components,
                                   std::forward<ComponentTypes>(components)...);
        addComponents_.push_back(info);
        return *this;
    }

    template <typename T>
    Commands &DestroyComponent(Entity entity) {
        auto idx = IndexGetter::Get<T>();
        destroyComponents_.emplace_back(ComponentDestroyInfo{entity, idx});

        return *this;
    }

    Commands &DestroyEntity(Entity entity) {
        destroyEntities_.push_back(entity);

        return *this;
    }

    template <typename T>
    Commands &SetResource(T &&resource) {
        auto index = IndexGetter::Get<T>();
        if (auto it = world_.resources_.find(index);
            it != world_.resources_.end()) {
            assertm("resource already exists", it->second.resource);
            it->second.resource = new T(std::move(std::forward<T>(resource)));
        } else {
            auto newIt = world_.resources_.insert_or_assign(
                index,
                World::ResourceInfo([](void *elem) { delete (T *)elem; }));
            newIt.first->second.resource =
                new T(std::move(std::forward<T>(resource)));
        }

        return *this;
    }

    template <typename T>
    Commands &RemoveResource() {
        auto index = IndexGetter::Get<T>();
        destroyResources_.push_back(
            ResourceDestroyInfo(index, [](void *elem) { delete (T *)elem; }));

        return *this;
    }

    void Execute() {
        for (const auto &info : destroyResources_) {
            removeResource(info);
        }
        for (auto e : destroyEntities_) {
            destroyEntity(e);
        }
        for (const auto &c : destroyComponents_) {
            destroyComponent(c);
        }

        for (auto &spawnInfo : spawnEntities_) {
            auto it = world_.entities_.emplace(spawnInfo.entity,
                                               World::ComponentContainer{});
            auto &componentContainer = it.first->second;
            for (auto &componentInfo : spawnInfo.components) {
                componentContainer[componentInfo.index] =
                    doSpawnWithoutType(spawnInfo.entity, componentInfo);
            }
        }

        for (auto &addInfo : addComponents_) {
            auto it = world_.entities_.find(addInfo.entity);
            if (it == world_.entities_.end()) {
                continue;
            }
            auto &componentContainer = it->second;
            for (auto &componentInfo : addInfo.components) {
                componentContainer[componentInfo.index] =
                    doSpawnWithoutType(addInfo.entity, componentInfo);
            }
        }
    }

private:
    World &world_;

    using DestroyFunc = void (*)(void *);

    struct ResourceDestroyInfo {
        uint32_t index;
        DestroyFunc destroy;

        ResourceDestroyInfo(uint32_t index, DestroyFunc destroy)
            : index(index), destroy(destroy) {}
    };

    using AssignFunc = std::function<void(void *)>;

    struct ComponentSpawnInfo {
        AssignFunc assign;
        World::Pool::CreateFunc create;
        World::Pool::DestroyFunc destroy;
        ComponentID index;
    };

    struct EntitySpawnInfo {
        Entity entity;
        std::vector<ComponentSpawnInfo> components;
    };

    struct EntityDestroyInfo {
        Entity entity;
    };

    struct ComponentDestroyInfo {
        Entity entity;
        ComponentID index;
    };

    std::vector<Entity> destroyEntities_;
    std::vector<ResourceDestroyInfo> destroyResources_;
    std::vector<EntitySpawnInfo> spawnEntities_;
    std::vector<EntitySpawnInfo> addComponents_;
    std::vector<ComponentDestroyInfo> destroyComponents_;

    template <typename T, typename... Remains>
    void doSpawn(std::vector<ComponentSpawnInfo> &spawnInfo, T &&component,
                 Remains &&...remains) {
        ComponentSpawnInfo info;
        info.index = IndexGetter::Get<T>();
        info.create = [](void) -> void * { return new T; };
        info.destroy = [](void *elem) { delete (T *)elem; };
        info.assign = [=](void *elem) { *((T *)elem) = component; };
        spawnInfo.push_back(info);

        if constexpr (sizeof...(Remains) != 0) {
            doSpawn<Remains...>(spawnInfo, std::forward<Remains>(remains)...);
        }
    }

    void *doSpawnWithoutType(Entity entity, ComponentSpawnInfo &info) {
        if (auto it = world_.componentMap_.find(info.index);
            it == world_.componentMap_.end()) {
            world_.componentMap_.insert_or_assign(
                info.index, World::ComponentInfo(info.create, info.destroy));
        }
        World::ComponentInfo &componentInfo = world_.componentMap_[info.index];
        void *elem = componentInfo.pool.Create();
        info.assign(elem);
        componentInfo.sparseSet.Add(entity);

        return elem;
    }

    void destroyEntity(Entity entity) {
        if (auto it = world_.entities_.find(entity);
            it != world_.entities_.end()) {
            for (auto &[id, component] : it->second) {
                auto &componentInfo = world_.componentMap_[id];
                componentInfo.pool.Destroy(component);
                componentInfo.sparseSet.Remove(entity);
            }
            world_.entities_.erase(it);
        }
    }

    void destroyComponent(const ComponentDestroyInfo &c) {
        if (auto it = world_.entities_.find(c.entity);
            it != world_.entities_.end()) {
            if (auto cit = it->second.find(c.index); cit != it->second.end()) {
                void *component = cit->second;
                it->second.erase(cit);
                if (auto iit = world_.componentMap_.find(c.index);
                    iit != world_.componentMap_.end()) {
                    iit->second.pool.Destroy(component);
                    iit->second.sparseSet.Remove(c.entity);
                }
            }
        }
    }

    void removeResource(const ResourceDestroyInfo &info) {
        if (auto it = world_.resources_.find(info.index);
            it != world_.resources_.end()) {
            info.destroy(it->second.resource);
            it->second.resource = nullptr;
        }
    }
};

class Resources final {
public:
    Resources(World &world) : world_(world) {}

    template <typename T>
    bool Has() const {
        auto index = IndexGetter::Get<T>();
        auto it = world_.resources_.find(index);
        return it != world_.resources_.end() && it->second.resource;
    }

    template <typename T>
    T &Get() {
        auto index = IndexGetter::Get<T>();
        return *((T *)world_.resources_.at(index).resource);
    }

private:
    World &world_;
};

// some query condition
enum class ConditionType {
    With,
    Option,
    Without,
};

// some condition

//! @brief query condition, all conditions must be true
//! @tparam ...Args  conditions, can be component or query condition
//! @see Option Without
template <typename... Args>
struct With {};

//! @brief query condition, any conditions be true will satisfy
//! @tparam ...Args  conditions, can be component or query condition
//! @see With Without
template <typename... Args>
struct Option {};

//! @brief query condition, all conditions be false will satisfy
//! @tparam ...Args  conditions, can be component or query condition
//! @see With Option
template <typename... Args>
struct Without {};

//! @brief query condition extractor, will extract condition arguments and condition type
//! @tparam query condition
//! @see With Option Without
template <typename T>
struct ConditionExtractor;

template <typename... Args>
struct ConditionExtractor<Without<Args...>> {
    using args = std::tuple<Args...>;
    static constexpr ConditionType type = ConditionType::Without;
};

template <typename... Args>
struct ConditionExtractor<With<Args...>> {
    using args = std::tuple<Args...>;
    static constexpr ConditionType type = ConditionType::With;
};

template <typename... Args>
struct ConditionExtractor<Option<Args...>> {
    using args = std::tuple<Args...>;
    static constexpr ConditionType type = ConditionType::Option;
};

template <typename T>
struct IsCondition {
    static constexpr bool value = false;
};

template <typename... Args>
struct IsCondition<With<Args...>> {
    static constexpr bool value = true;
};

template <typename... Args>
struct IsCondition<Option<Args...>> {
    static constexpr bool value = true;
};

template <typename... Args>
struct IsCondition<Without<Args...>> {
    static constexpr bool value = true;
};

//! @brief judge if template T is a query condition
//! @tparam T 
//! @see Without With Option
template <typename T>
constexpr auto IsConditionV = IsCondition<T>::value;

//! @brief condition querier, can accept conditions
//! @see Without With Option
class Querier final {
public:
    Querier(World& world): world_(world) {}

    /* IMPROVE: currently it iterate all entities,
                it don't take advantage of the efficiency of the sparseset.
                don't forget to fix it later */
    template <typename T>
    std::vector<Entity> Query() {
        std::vector<Entity> entities;
            for (auto& [entity, _] : world_.entities_) {
            if constexpr(IsConditionV<T>) {
                if (Has<T>(entity)) {
                    entities.push_back(entity);
                }
            } else {
                if (queryExists<T>(entity)) {
                    entities.push_back(entity);
                }
            }
        }
        return entities;
    }

    template <typename T>
    bool Has(ecs::Entity entity) const {
        return queryCondition<T>(entity);
    }

    template <typename T>
    T &Get(Entity entity) {
        auto index = IndexGetter::Get<T>();
        return *((T *)world_.entities_[entity][index]);
    }

private:
    World& world_;

    template <typename T>
    bool queryCondition(Entity entity) const {
        if constexpr (IsConditionV<T>) {
            using extractor = ConditionExtractor<T>;
            return doQueryCondition<0, extractor::args>(entity, extractor::type);
        } else {
            return queryExists<T>(entity);
        }
    }

    template <size_t Idx, typename TupleT>
    bool doQueryCondition(Entity entity, ConditionType type) const {
        if constexpr (Idx < std::tuple_size_v<TupleT>) {
            bool result = queryConditionElem<Idx, TupleT>(entity);
            switch (type) {
                case ConditionType::With:
                    if (!result) {
                        return false;
                    } else {
                        return doQueryCondition<Idx + 1, TupleT>(entity, type);
                    }
                case ConditionType::Option:
                    if (result) {
                        return true;
                    } else {
                        return doQueryCondition<Idx + 1, TupleT>(entity, type);
                    }
                case ConditionType::Without:
                    if (result) {
                        return false;
                    } else {
                        return doQueryCondition<Idx + 1, TupleT>(entity, type);
                    }
            }
            return false;
        }
        return true;
    }

    template <size_t Idx, typename TupleT>
    bool queryConditionElem(Entity entity) const {
        using T = std::tuple_element_t<Idx, TupleT>;
        if constexpr (IsConditionV<T>) {
            return queryCondition<T>(entity);
        } else {
            return queryExists<T>(entity);
        }
    }

    template <typename T>
    bool queryExists(Entity entity) const {
        auto index = IndexGetter::Get<T>();
        auto &componentContainer = world_.entities_[entity];
        return componentContainer.find(index) != componentContainer.end();
    }
};

inline void World::Startup() {
    for (auto &plugins : pluginsList_) {
        plugins->Build(this);
    }

    for (auto sys : startupSystems_) {
        Commands commands{*this};
        sys(commands, Resources{*this});
        commands.Execute();
    }
}

//! @brief a help function to preorder node tree
inline void PreorderVisit(std::optional<Entity> parent, Entity entity,
                          World &world, std::vector<Commands> commandList,
                          Querier querier, Resources res, Events &events,
                          HierarchyUpdateSystem system) {
    Commands commands{world};
    system(parent, entity, commands, querier, res, events);
    commandList.push_back(commands);

    assert(querier.Has<Node>(entity));

    auto &node = querier.Get<Node>(entity);

    for (auto &child : node.children) {
        PreorderVisit(entity, child, world, commandList, querier, res, events,
                      system);
    }
}

inline void World::Update() {
    std::vector<Commands> commandList;

    Events events;
    Querier querier{*this};

    // find all root entity in hierarchy
    static std::vector<Entity> nodeEntities;
    static std::vector<Entity> rootNodeEntity;
    nodeEntities.clear();
    rootNodeEntity.clear();
    nodeEntities = querier.Query<Node>();
    for (auto entity : nodeEntities) {
        if (!querier.Get<Node>(entity).parent) {
            rootNodeEntity.push_back(entity);
        }
    }

    for (auto& sys : updateSystems_) {
        auto system = std::get_if<EachElemUpdateSystem>(&sys);
        if (system) {
            Commands commands{*this};
            (*system)(commands, Querier{*this}, Resources{*this}, events);
            commandList.push_back(commands);
        } else {
            auto hierarchySystem = std::get_if<HierarchyUpdateSystem>(&sys);
            for (auto root : rootNodeEntity) {
                PreorderVisit(std::nullopt, root, *this, commandList,
                                Querier{*this}, Resources{*this}, events,
                                *hierarchySystem);
            }
        }
        /* FIXME: want to use compile-if, but can't determine system type
        std::visit(
            [&](auto &&system) {
                using T = std::decay_t<decltype(system)>;
                if constexpr (std::is_same_v<T, EachElemUpdateSystem>) {
                    Commands commands{*this};
                    sys(commands, Querier{*this}, Resources{*this}, events);
                    commandList.push_back(commands);
                } else if constexpr (std::is_same_v<T, HierarchyUpdateSystem>) {
                    for (auto root : rootNodeEntity) {
                        PreorderVisit(std::nullopt, root, *this, commandList,
                                      Querier{*this}, Resources{*this}, events,
                                      system);
                    }
                } else {
                    static_assert(std::always_false_v<T> "unknown ecs system type");
                }
            },
            sys);
        */
    }
    events.removeAllEvents();
    events.addAllEvents();

    for (auto &commands : commandList) {
        commands.Execute();
    }
}

template <typename T>
World &World::SetResource(T &&resource) {
    Commands commands(*this);
    commands.SetResource(std::forward<T>(resource));

    return *this;
}

template <typename T>
T *World::GetResource() {
    Resources resources(*this);
    if (!resources.Has<T>()) {
        return nullptr;
    } else {
        return &resources.Get<T>();
    }
}

}  // namespace ecs