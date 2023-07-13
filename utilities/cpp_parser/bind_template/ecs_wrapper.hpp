#pragma once

#include "core/ecs.hpp"
#include "luabind/refl-codes.hpp"

#define DECL_CMDS_ADD_COMP(name, clazz) \
void Add ## name (ecs::Entity entity, clazz & comp) { \
    cmds_.AddComponent<clazz>(entity, std::move(comp)); \
}

#define DECL_CMDS_DESTROY_COMP(name, clazz) \
void Destroy ## name (ecs::Entity entity, clazz & comp) { \
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

    {{ DECL_ADD_COMP }}
    {{ DECL_DESTROY_COMP }}

private:
    ecs::Commands& cmds_;
};

#undef DECL_CMDS_ADD_COMP
#undef DECL_CMDS_DESTROY_COMP


#define DECL_RES_GET(name, clazz) \
clazz& Get ## name () { \
    res_.Get<clazz>(); \
}

class [[refl, luabind]] ResourcesWrapper final {
public:
    [[norefl]] ResourcesWrapper(ecs::Resources& res): res_(res) {}

    {{ DECL_RES }}

private:
    ecs::Resources& res_;
};

#undef DECL_RES_GET


#define DECL_QUERIER_QUERY(name, clazz) \
std::vector<ecs::Entity> Query ## name () { \
    return res_.Query<clazz>(); \
}

#define DECL_QUERIER_GET(name, clazz) \
auto& Get ## name (ecs::Entity entity) { \
    return res_.Get<clazz>(entity); \
}

#define DECL_QUERIER_HAS(name, clazz) \
bool Has ## name (ecs::Entity entity) { \
    return res_.Has<clazz>(entity); \
}

class [[refl, luabind]] QueriesWrapper final {
public:
    [[norefl]] QueriesWrapper(ecs::Queries& queries): queries_(queries) {}

    {{ DECL_QUERIER_QUERY }}
    {{ DECL_QUERIER_GET }}
    {{ DECL_QUERIER_HAS }}

private:
    ecs::Queries& queries_;
};

#undef DECL_QUERIER_QUERY
#undef DECL_QUERIER_GET
#undef DECL_QUERIER_HAS
