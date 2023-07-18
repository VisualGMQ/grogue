#pragma once

#include "core/ecs.hpp"
{{ INCLUDE_FILES }}

#define DECL_CMDS_ADD_COMP(name, clazz) \
void AddComponent(ecs::Entity entity, clazz & comp) { \
    cmds_.AddComponent<clazz>(entity, std::move(comp)); \
}

#define DECL_CMDS_DESTROY_COMP(name, clazz) \
void DestroyComponent(ecs::Entity entity, clazz & comp) { \
    cmds_.DestroyComponent<clazz>(entity); \
}


class [[refl, luabind]] CommandsWrapper final {
public:
    [[norefl]] CommandsWrapper(ecs::Commands& cmds): cmds_(cmds) {}

    ecs::Entity Spawn() {
        return cmds_.SpawnAndReturn();
    }

    void DestroyEntity(ecs::Entity entity) {
        cmds_.DestroyEntity(entity);
    }

    auto& Raw() { return cmds_; }
    auto& Raw() const { return cmds_; }

    {{ DECL_ADD_COMP }}
    {{ DECL_DESTROY_COMP }}

private:
    ecs::Commands& cmds_;
};

#undef DECL_CMDS_ADD_COMP
#undef DECL_CMDS_DESTROY_COMP


#define DECL_RES_GET(name, clazz) \
clazz& Get ## name () { \
    return res_.Get<clazz>(); \
}

class [[refl, luabind]] ResourcesWrapper final {
public:
    [[norefl]] ResourcesWrapper(ecs::Resources& res): res_(res) {}

    auto& Raw() { return res_; }
    auto& Raw() const { return res_; }

    {{ DECL_RES }}

private:
    ecs::Resources& res_;
};

#undef DECL_RES_GET


#define DECL_QUERIER_QUERY(name, clazz) \
std::vector<ecs::Entity> Query ## name () { \
    return querier_.Query<clazz>(); \
}

#define DECL_QUERIER_GET(name, clazz) \
auto& Get ## name (ecs::Entity entity) { \
    return querier_.Get<clazz>(entity); \
}

#define DECL_QUERIER_HAS(name, clazz) \
bool Has ## name (ecs::Entity entity) { \
    return querier_.Has<clazz>(entity); \
}

class [[refl, luabind]] QuerierWrapper final {
public:
    [[norefl]] QuerierWrapper(ecs::Querier& querier): querier_(querier) {}

    auto& Raw() { return querier_; }
    auto& Raw() const { return querier_; }

    {{ DECL_QUERIER_QUERY }}
    {{ DECL_QUERIER_GET }}
    {{ DECL_QUERIER_HAS }}

private:
    ecs::Querier& querier_;
};

#undef DECL_QUERIER_QUERY
#undef DECL_QUERIER_GET
#undef DECL_QUERIER_HAS

class [[refl, luabind]] EventsWrapper final {
public:
    [[norefl]] EventsWrapper(ecs::Events& events): events_(events) {}

    auto& Raw() { return events_; }
    auto& Raw() const { return events_; }

private:
    ecs::Events& events_;
};