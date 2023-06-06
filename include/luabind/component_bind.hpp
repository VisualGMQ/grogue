#pragma once

#include "core/pch.hpp"
#include "app/lua.hpp"
#include "app/transform.hpp"
#include "app/scene.hpp"
#include "app/sprite.hpp"
#include "app/font.hpp"
#include "app/ui.hpp"

namespace lua_bind {

using ui::RectTransform;
using ui::Panel;

#define IMPL_QUERY_FUNC(type) \
std::vector<ecs::Entity> Query ## type ## () { \
    return querier_.Query<type>(); \
}

#define IMPL_HAS_FUNC(type) \
bool Has ## type ## (ecs::Entity entity) { \
    return querier_.Has<type>(entity); \
}

#define IMPL_GET_FUNC(type) \
auto Get ## type ## (ecs::Entity entity) { \
    return querier_.Get<type>(entity); \
} 

class LuaQuerier final {
public:
    LuaQuerier(ecs::Querier querier): querier_(querier) {}

    IMPL_QUERY_FUNC(SpriteBundle)
    IMPL_QUERY_FUNC(NodeTransform)
    IMPL_QUERY_FUNC(RectTransform)
    IMPL_QUERY_FUNC(Transform)
    IMPL_QUERY_FUNC(Panel)
    IMPL_QUERY_FUNC(Node)

    IMPL_HAS_FUNC(SpriteBundle)
    IMPL_HAS_FUNC(NodeTransform)
    IMPL_HAS_FUNC(RectTransform)
    IMPL_HAS_FUNC(Transform)
    IMPL_HAS_FUNC(Panel)
    IMPL_HAS_FUNC(Node)

    IMPL_GET_FUNC(SpriteBundle)
    IMPL_GET_FUNC(NodeTransform)
    IMPL_GET_FUNC(RectTransform)
    IMPL_GET_FUNC(Transform)
    IMPL_GET_FUNC(Panel)
    IMPL_GET_FUNC(Node)

private:
    ecs::Querier querier_;
};

#undef IMPL_QUERY_FUNC
#undef IMPL_HAS_FUNC
#undef IMPL_GET_FUNC

void BindLuaQuerier(LuaScript&);
void BindComponents(LuaScript&);
void BindSimpleTypes(LuaScript&);

}