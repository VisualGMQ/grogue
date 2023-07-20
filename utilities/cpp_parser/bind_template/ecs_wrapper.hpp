#pragma once

#include "core/ecs.hpp"
{{ INCLUDE_FILES }}

/* NOTE: don't ask me why forward declare this. Remove this will make libclang can't parse sol::object.
 If you add lib sol include path to libclang, it will cause other file parse problem*/
namespace sol {
	template <bool>
	class basic_reference;
	using reference = basic_reference<false>;
	template <typename base_type>
	class basic_object;
	using object = basic_object<reference>;
}
using SolObject = sol::object;

#define DECL_CMDS_ADD_COMP(clazz) \
void AddComponent(ecs::Entity entity, ::ecs::ComponentID id, clazz & comp) { \
    cmds_.AddComponentByID(entity, id, std::move(comp)); \
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

    void DestroyComponent(ecs::Entity entity, ::ecs::ComponentID id) {
        cmds_.DestroyComponentByID(entity, id);
    }

    void SetResource(::ecs::ComponentID id, sol::object& res) {
        cmds_.SetLuaResourceByID(id, res);
    }

    void RemoveResource(::ecs::ComponentID id) {
        cmds_.RemoveLuaResource(id);
    }

    DECL_CMDS_ADD_COMP(SolObject)

    {{ DECL_ADD_COMP }}

private:
    ecs::Commands& cmds_;
};

#undef DECL_CMDS_ADD_COMP


#define DECL_RES_GET(name, clazz) \
clazz& Get ## name () { \
    return res_.Get<clazz>(); \
}

class [[refl, luabind]] ResourcesWrapper final {
public:
    [[norefl]] ResourcesWrapper(ecs::Resources& res): res_(res) {}

    auto& Raw() { return res_; }
    auto& Raw() const { return res_; }

    bool Has(::ecs::ComponentID id) const {
        return res_.Has(id);
    }

    sol::object Get(::ecs::ComponentID id, sol::this_state s) {
        sol::state_view lua(s);

        {{ DECL_RES }}

        return sol::make_object(lua, res_.GetLuaRes(id));
    }

private:
    ecs::Resources& res_;
};

#undef DECL_RES_GET

#define DECL_QUERIER_GET(name, clazz) \
clazz& Get ## name (ecs::Entity entity) { \
    return querier_.Get<clazz>(entity); \
}

class [[refl, luabind]] QuerierWrapper final {
public:
    [[norefl]] QuerierWrapper(ecs::Querier& querier): querier_(querier) {}

    auto& Raw() { return querier_; }
    auto& Raw() const { return querier_; }

    std::vector<ecs::Entity> Query(ecs::ComponentID id) {
        return querier_.QueryByID(id);
    }

    bool Has(ecs::Entity entity, ecs::ComponentID id) {
        return querier_.HasByID(entity, id);
    }

    sol::object Get(ecs::Entity entity, ecs::ComponentID id, sol::this_state s) {
        sol::state_view lua(s);

        {{ DECL_QUERIER_CONVENIENCE_GET }}

        return GetLuaComponent(entity, id);
    }

    sol::object& GetLuaComponent(::ecs::Entity entity, ::ecs::ComponentID id) {
		return querier_.GetLuaComponent(entity, id);
	}

    DECL_QUERIER_GET(SolObject, SolObject)

    {{ DECL_QUERIER_GET }}

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