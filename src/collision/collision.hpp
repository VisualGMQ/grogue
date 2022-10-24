#pragma once

#include "engine/engine.hpp"
#include "others/in_out.hpp"

bool LineIntersectLine(const engine::Vec2& pA1, const engine::Vec2& pA2,
                       const engine::Vec2& pB1, const engine::Vec2& pB2, OUT float* s);

bool LineIntersectRect(const engine::Vec2& pStart, const engine::Vec2& pEnd, const engine::Rect& rect,
                       engine::Vec2* n, float* s);

bool IsPointInRect(const engine::Vec2& p, const engine::Rect& r);

struct CollisionResult {
    engine::Vec2 normal;
    float s;
};

bool MinkowskiCollide(const engine::Rect& r1, const engine::Rect& r2,
                      const engine::Vec2& v1, const engine::Vec2& v2,
                      OUT CollisionResult& result);