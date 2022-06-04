#include "physical/collider.hpp"

namespace grogue::physical {

BoxCollider::BoxCollider(const core::Vec2& pos,
                         const core::Size& size,
                         const core::Vec2& offset)
: offset(offset), pos(pos), size(size) {}

bool IsCollide(const BoxCollider& c1, const BoxCollider& c2) {
    return core::IsRectsIntersect(core::Rect(c1.pos, c1.size),
                                  core::Rect(c2.pos, c2.size));
}

CollideResult::CollideResult(const BoxCollider& c1, const BoxCollider& c2)
: c1(c1), c2(c2), msv(0, 0) {}

CollideResult Collide(const BoxCollider& c1, const BoxCollider& c2) {
    CollideResult result(c1, c2);
    auto intersection = core::RectsIntersect(core::Rect(c1.pos, c1.size),
                                             core::Rect(c2.pos, c2.size));
    if (intersection.w <= intersection.h) {
        result.msv.x = intersection.w;
    }
    if (intersection.h <= intersection.w) {
        result.msv.y = intersection.h;
    }
    // NOTE don't have CCD detect
    return result;
}

}
