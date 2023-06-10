#pragma once

#include "core/pch.hpp"
#include "app/lua.hpp"
#include "app/transform.hpp"
#include "app/scene.hpp"
#include "app/sprite.hpp"
#include "app/font.hpp"
#include "app/ui.hpp"
#include "game/types.hpp"

namespace lua_bind {

using ui::RectTransform;
using ui::Panel;

#define IMPL_QUERY_FUNC(type) \
std::vector<ecs::Entity> Query ## type() { \
    return querier_.Query<type>(); \
}

#define IMPL_HAS_FUNC(type) \
bool Has ## type(ecs::Entity entity) { \
    return querier_.Has<type>(entity); \
}

#define IMPL_GET_FUNC(type) \
auto Get ## type(ecs::Entity entity) { \
    return querier_.Get<type>(entity); \
} 

#define IMPL_FUNC(type) \
IMPL_QUERY_FUNC(type) \
IMPL_HAS_FUNC(type) \
IMPL_GET_FUNC(type) \

class QuerierWrapper final {
public:
    friend class SignalManagerWrapper;

    QuerierWrapper(ecs::Querier querier): querier_(querier) {}

    // engine related
    IMPL_FUNC(SpriteBundle)
    IMPL_FUNC(RectTransform)
    IMPL_FUNC(NodeTransform)
    IMPL_FUNC(Transform)
    IMPL_FUNC(Panel)
    IMPL_FUNC(Node)

    // game related
    IMPL_FUNC(BackpackUIPanel)
    IMPL_FUNC(Player)

private:
    ecs::Querier querier_;
};

#undef IMPL_QUERY_FUNC
#undef IMPL_HAS_FUNC
#undef IMPL_GET_FUNC
#undef IMPL_FUNC

void BindQuerierWrapper(LuaScript&);
void BindComponents(LuaScript&);
void BindSimpleTypes(LuaScript&);

}