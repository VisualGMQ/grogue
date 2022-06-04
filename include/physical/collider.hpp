#pragma once

#include "core/mathf.hpp"

namespace grogue::physical {

struct BoxCollider final {
    core::Vec2 offset;
    core::Vec2 pos;
    core::Size size;

    BoxCollider(const core::Vec2& pos,
                const core::Size& size,
                const core::Vec2& offset = core::Vec2(0, 0));
};

bool IsCollide(const BoxCollider&, const BoxCollider&);

struct CollideResult final {
    const BoxCollider& c1;
    const BoxCollider& c2;
    core::Vec2 msv;

    CollideResult(const BoxCollider& c1, const BoxCollider& c2);
};

CollideResult Collide(const BoxCollider& c1, const BoxCollider& c2);

}

