#include "collision.hpp"

bool LineIntersectLine(const engine::Vec2& pA1, const engine::Vec2& pA2,
                       const engine::Vec2& pB1, const engine::Vec2& pB2, /* OUT */ float* s) {
    auto dir1 = pA2 - pA1;
    auto dir2 = pB2 - pB1;
    float divisor = dir1.x * (-dir2.y) - (-dir2.x) * dir1.y;
    if (std::abs(divisor) <= std::numeric_limits<decltype(divisor)>::epsilon()) {
        return false;
    }

    auto ba = pB1 - pA1;
    float s1 = (ba.x * dir2.y - dir2.x * ba.y) / divisor;
    float s2 = (dir1.x * ba.y - ba.x * dir1.y) / divisor;
    if (s) {
        *s = s1;
    }
    if (s1 >= 0 && s1<= 1 && s2 >= 0 && s2 <= 1) {
        return true;
    }
    return false;
}

bool LineIntersectRect(const engine::Vec2& pStart, const engine::Vec2& pEnd, const engine::Rect& rect,
                       engine::Vec2* n, float* s) {
    if (IsPointInRect(pStart, rect)) { return false; }

    auto topLeft = rect.position;
    auto topRight = rect.position + engine::Vec2(rect.size.w, 0);
    auto bottomLeft = rect.position + engine::Vec2(0, rect.size.h);
    auto bottomRight = rect.position + engine::Vec2(rect.size.w, rect.size.h);

    float oldS = std::numeric_limits<float>::max();
    float sPart;
    engine::Vec2 norm;
    bool intersected = false;

    #define _CHECK_MIN_S(rectP1, rectP2, normal) \
        if (LineIntersectLine(pStart, pEnd, rectP1, rectP2, &sPart)) { \
            if (sPart < oldS) { \
                oldS = sPart; \
                norm = normal; \
            } \
            intersected = true; \
        }

    _CHECK_MIN_S(topLeft, topRight, engine::Vec2(0, -1))
    _CHECK_MIN_S(topLeft, bottomLeft, engine::Vec2(-1, 0))
    _CHECK_MIN_S(topRight, bottomRight, engine::Vec2(1, 0))
    _CHECK_MIN_S(bottomLeft, bottomRight, engine::Vec2(0, 1))

    #undef _CHECK_MIN_S

    if (s) *s = oldS;
    if (n) *n = norm;

    return intersected;
}

bool IsPointInRect(const engine::Vec2& p, const engine::Rect& r) {
    return p.x >= r.position.x && p.x <= r.position.x + r.size.w &&
           p.y >= r.position.y && p.y <= r.position.y + r.size.h;
}

bool MinkowskiCollide(const engine::Rect& r1, const engine::Rect& r2,
                      const engine::Vec2& v1, const engine::Vec2& v2,
                      OUT CollisionResult& result) {
    engine::Rect rect(r1.position.x - r2.size.w / 2.0, r1.position.y - r2.size.h / 2.0,
                      r1.size.w + r2.size.w, r1.size.h + r2.size.h);
    engine::Vec2 v = v2 - v1;
    engine::Vec2 p = r1.position + r1.size / 2.0;

    return LineIntersectRect(p, p + v, rect, &result.normal, &result.s);
}