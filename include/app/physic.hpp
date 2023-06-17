#pragma once

#include "core/pch.hpp"
#include "app/timer.hpp"
#include "app/renderer.hpp"
#include "app/transform.hpp"
#include "app/scene.hpp"

namespace physic {

//! @brief component for a single particle(has position, no rotation)
struct LUA_BIND_COMPONENT Particle final {
    float massInv = 1.0;  //!< @brief reciprocal of mass
    math::Vector2 pos;
    math::Vector2 vel;
    math::Vector2 acc;
    math::Vector2 force;

    static Particle Create(const math::Vector2& pos, float massInv) {
        return Particle{massInv, pos};
    }
};

//! @brief component for a rigid body(has position, rotation)
struct LUA_BIND_COMPONENT RigidBody final {
    Particle particle;
    float rotation;
    float angVel;   //!< @brief angular velocity
};

enum ShapeType {
    ShapeAABB,
    ShapeCircle,
};

struct LUA_BIND Shape {
    Shape(ShapeType type): type_(type) {}
    Shape(ShapeType type, const math::Vector2& offset): type_(type), offset(offset) {}
    virtual ~Shape() = default;

    math::Vector2 GetCenter() const { return center; }
    ShapeType GetType() const { return type_; }

    math::Vector2 center;
    math::Vector2 offset;

private:
    ShapeType type_;
};

struct LUA_BIND AABB final: public Shape {
    math::Vector2 halfLen = {1, 1};

    AABB(): Shape(ShapeType::ShapeAABB) {}
    AABB(const math::Vector2& offset, const math::Vector2& halfLen): Shape(ShapeType::ShapeAABB, offset), halfLen(halfLen) {}
};

struct LUA_BIND Circle final: public Shape {
    float radius = 1.0;

    Circle(): Shape(ShapeType::ShapeCircle) {}
    Circle(const math::Vector2& offset, float radius): Shape(ShapeType::ShapeCircle, offset), radius(radius) {}
};

struct Contact final {
    math::Vector2 point;
    math::Vector2 normal;   //!< the normal of contact point, from shape2 edge, point out of shape2
                            //!< Specially, for two circle, it store normalize of (c2.center - c1.center), and contact point always 1 or 0
    float penetration = 0;
};

struct Manifold final {
    Shape* shape1 = nullptr;
    Shape* shape2 = nullptr;
    ecs::Entity entity1 = 0;
    ecs::Entity entity2 = 0;
    Contact contacts[2];    //!< in current code, max contact count is 2, but may change later
    size_t contactNum = 0;  //!< real contact count in `contacts`
};

Manifold GenManifoldCircle(const Circle&, const Circle&);
Manifold GenManifoldCircleAABB(const Circle&, const AABB&);
Manifold GenManifoldAABB(const AABB&, const AABB&);

//! @brief get two line intersect t1, t2(when line: p = s + t * d)
//! @param s1 arbitrary point on line1
//! @param d1 direction of line1(don't need normalize)
//! @param s2 arbitrary point on line2
//! @param d2 direction of line2(don't need normalize)
//! @return return t1 and t2 if intersect
inline std::optional<std::pair<float, float>> IntersectLine(
    const math::Vector2& s1, const math::Vector2& d1, const math::Vector2& s2,
    const math::Vector2& d2);
inline std::optional<std::pair<float, float>> IntersectLineCircle(
    const Circle& c, const math::Vector2& s, const math::Vector2& d);
inline std::optional<std::pair<math::Vector2, math::Vector2>> IntersectCircle(const Circle&, const Circle&);
inline bool IsLineParallel(const math::Vector2& v1, const math::Vector2& v2);
inline bool IsAABBIntersect(const AABB& a, const AABB& b);
inline bool IsCircleIntersect(const Circle& a, const Circle& b);
inline bool IsCircleAABBIntersect(const Circle& a, const AABB& b);
//! @brief judge if circle intersect line
//! @param c circle
//! @param s arbitrary point on line
//! @param d direction of line
inline bool IsCircleLineIntersect(const Circle& c, const math::Vector2& s, const math::Vector2& d);
inline float Pt2LineDist(const math::Vector2& c, const math::Vector2& s, const math::Vector2& d);
//! @brief find nearest point on aabb by given point
inline math::Vector2 NearestPtOnAABB(const math::Vector2& pt, const AABB& aabb);
//! @brief find nearest point on circle by given point
inline math::Vector2 NearestPtOnCircle(const math::Vector2& pt, const Circle& circle);
//! @brief find nearest point on line by given point
//! @param p given point
//! @param s arbitrary point on line
//! @param s arbitrary point on line
//! @return t on line pt = s + t * d
inline float NearestPtOnLine(const math::Vector2& p, const math::Vector2& s, const math::Vector2& d);

//! @brief component for collide shape

LUA_BIND;
enum CollideUsage {
    Collide = 0x01,     //<! @brief collide with others, handle collide manifold
    Listener = 0x02,    //<! @brief collide with others without handle collide manifold
    Both = 0x03,
};

struct LUA_BIND_COMPONENT CollideShape final {
    std::shared_ptr<Shape> shape;
    CollideUsage usage = CollideUsage::Collide;
};

using ForceGenerator = std::function<void(Particle&, Time::TimeType)>;

class Grid final {
public:
    Grid(int cellW, int cellH, int col, int row)
        : grid_(col, row), cellW_(cellW), cellH_(cellH), maxW_(cellW * col), maxH_(cellH * row)  {
        assert(cellW > 0 && cellH > 0 && col > 0 && row > 0);
    }

    void Add(ecs::Entity, const math::Rect& rect);
    void Change(ecs::Entity, const math::Rect& oldRect,
                const math::Rect& newRect);
    void Remove(ecs::Entity, const math::Rect& rect);

    bool IsInGrid(const math::Rect& rect) {
        return math::Rect(0, 0, maxW_, maxH_).IsIntersect(rect);
    }

    std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> CalcContainedRange(const math::Rect& r);

    auto& GetGrid() { return grid_; }

private:
    math::HeapMatrix<std::vector<ecs::Entity>> grid_;
    uint32_t cellW_;
    uint32_t cellH_;
    uint32_t maxW_;
    uint32_t maxH_;
};

struct LUA_BIND_RESOURCE PhysicWorld final {
    Grid grid;
    std::vector<ForceGenerator> forceGenerators;
    std::vector<Manifold> manifolds;
};

void UpdateParticleSystem(ecs::Commands&, ecs::Querier, ecs::Resources,
                          ecs::Events&);
void DoCollideSystem(ecs::Commands&, ecs::Querier, ecs::Resources,
                     ecs::Events&);

void UpdatePos2Particle(ecs::Commands&, ecs::Querier, ecs::Resources,
                        ecs::Events&);
void UpdatePos2Entity(ecs::Commands&, ecs::Querier, ecs::Resources,
                      ecs::Events&);
}  // namespace physic