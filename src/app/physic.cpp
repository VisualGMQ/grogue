#include "app/physic.hpp"

namespace physic {

//! @brief solve binary equation
//! equation in:
//! $$
//! a_1 * x + b_1 * y = c_1
//! a_2 * x + b_2 * y = c_2
//! $$
//! @return x, y
inline std::optional<std::pair<float, float>> solveBinaryEquation(
    float a1, float b1, float c1, float a2, float b2, float c2) {
    float det = a1 * b2 - a2 * b1;
    if (std::abs(det) <= 0.0000001) {
        return std::nullopt;
    }

    return std::make_pair<float, float>((c1 * b2 - c2 * b1) / det,
                                        (a1 * c2 - a2 * c1) / det);
}

std::optional<std::pair<float, float>> IntersectLine(const math::Vector2& s1,
                                                     const math::Vector2& d1,
                                                     const math::Vector2& s2,
                                                     const math::Vector2& d2) {
    if (IsLineParallel(d1, d2)) {
        return std::nullopt;
    }

    auto s = s2 - s1;
    return solveBinaryEquation(d1.x, -d2.x, s.x, d1.y, -d2.y, s.y);
}

std::optional<std::pair<float, float>> IntersectLineCircle(
    const Circle& cir, const math::Vector2& s, const math::Vector2& d) {
    auto q = s - cir.center;
    float a = d.LengthSqrd();
    float b = 2.0 * d.Dot(q);
    float c = q.LengthSqrd() - cir.radius * cir.radius;
    
    float delta = b * b - 4.0 * a * c;
    if (delta < 0) {
        return std::nullopt;
    } else if (delta == 0) {
        float deltaSqrt = std::sqrt(delta);
        float t = (-b + deltaSqrt) / (2.0 * a);
        return std::pair(t, t);
    } else {
        float deltaSqrt = std::sqrt(delta);
        return std::pair<float, float>((-b + deltaSqrt) / (2.0 * a), (-b - deltaSqrt) / (2.0 * a));
    }
}

std::optional<std::pair<math::Vector2, math::Vector2>> IntersectCircle(const Circle& c1, const Circle& c2) {
    if (!IsCircleIntersect(c1, c2)) {
        return std::nullopt;
    }

    auto dir = c2.center - c1.center;
    float lenSqrd = dir.LengthSqrd();

    float cos = (lenSqrd + c1.radius * c1.radius - c2.radius * c2.radius) / (2.0 * c1.radius * std::sqrt(lenSqrd));
    float sin = std::sqrt(1.0 - cos * cos);
    math::Vector2 norm(1.0, 0.0);
    if (std::abs(dir.x) > 0.000001) {
        norm = math::Normalize(math::Vector2(-dir.y / dir.x, 1.0)) * sin * c1.radius;
    }
    math::Vector2 d = c1.center + math::Normalize(dir) * cos * c1.radius;

    return std::make_pair<math::Vector2, math::Vector2>(d + c1.center + norm, d + c1.center - norm);
}

inline float Pt2LineDist(const math::Vector2& p, const math::Vector2& s, const math::Vector2& d) {
    auto d2 = p - s;
    float len = d2.Length();
    if (len == 0) {
        return true;
    }

    return std::abs(d2.Cross(d) / len);
}

bool IsCircleLineIntersect(const Circle& c, const math::Vector2& s, const math::Vector2& d) {
    return Pt2LineDist(c.center, s, d) < c.radius;
}

bool IsLineParallel(const math::Vector2& v1, const math::Vector2& v2) {
    return std::abs(v1.Cross(v2)) <= 0.0000001;
}

bool IsAABBIntersect(const AABB& a, const AABB& b) {
    auto aMin = a.center - a.halfLen;
    auto aMax = a.center + a.halfLen;
    auto bMin = b.center - b.halfLen;
    auto bMax = b.center + b.halfLen;
    return !(aMax.x <= bMin.x || aMax.y <= bMin.y ||
             bMax.x <= aMin.x || bMax.y <= aMin.y);
}

bool IsCircleIntersect(const Circle& a, const Circle& b) {
    float rSum = a.radius + b.radius;
    float rSub = a.radius - b.radius;
    float lenSqrd = (a.center - b.center).LengthSqrd();
    return lenSqrd < rSum * rSum && lenSqrd > rSub * rSub;
}

bool IsCircleAABBIntersect(const Circle& a, const AABB& b) {
    auto nearestPt = NearestPtOnAABB(a.center, b);
    return (nearestPt - a.center).LengthSqrd() < a.radius * a.radius;
}

math::Vector2 NearestPtOnAABB(const math::Vector2& pt, const AABB& aabb) {
    auto min = aabb.center - aabb.halfLen;
    auto max = aabb.center + aabb.halfLen;
    if (pt.x > min.x && pt.x < max.y && pt.y > min.y && pt.y < max.x) {
        return pt;
    }
    return math::Vector2(std::clamp<float>(pt.x, min.x, max.x),
                         std::clamp<float>(pt.y, min.y, max.y));
}

math::Vector2 NearestPtOnCircle(const math::Vector2& pt, const Circle& circle) {
    return math::Normalize(pt - circle.center) * circle.radius + circle.center;
}

float NearestPtOnLine(const math::Vector2& p, const math::Vector2& s,
                              const math::Vector2& d) {
    return (p - s).Dot(d) / d.LengthSqrd();
}

//! @brief update one particle
//! @param particle
//! @param elapse elapsed time, in milliseconds
void updateOneParticle(Particle& particle, Time::TimeType elapse, std::vector<ForceGenerator>& forceGens) {
    double t = elapse / 1000.0;

    for (auto& generator : forceGens) {
        generator(particle, elapse);
    }

    particle.acc += particle.force * particle.massInv;
    particle.vel += particle.acc * t;
    if (particle.vel.LengthSqrd() <= 0.00001) {
        particle.vel.Set(0, 0);
    }
    particle.pos = particle.pos + particle.vel * t + 0.5 * particle.acc * t * t;

    particle.acc.Set(0, 0);
    particle.force.Set(0, 0);
}

void UpdateParticleSystem(ecs::Commands&, ecs::Querier querier, ecs::Resources res,
                          ecs::Events&) {
    auto entities = querier.Query<Particle>();
    auto& physicConfig = res.Get<PhysicWorld>();

    for (auto entity : entities) {
        auto& particle = querier.Get<Particle>(entity);
        updateOneParticle(particle, 33, physicConfig.forceGenerators);
    }

    entities = querier.Query<RigidBody>();
    for (auto entity : entities) {
        auto& rigid = querier.Get<RigidBody>(entity);
        updateOneParticle(rigid.particle, 33, physicConfig.forceGenerators);
    }
}

void updateOnePos(ecs::Entity entity, ecs::Querier querier, const math::Vector2& pos) {
    if (querier.Has<Transform>(entity)) {
        auto& transform = querier.Get<Transform>(entity);
        transform.position = pos;
    }
    if (querier.Has<NodeTransform>(entity)) {
        auto& transform = querier.Get<NodeTransform>(entity);
        transform.localTransform.position += pos - transform.globalTransform.position;
    }
}

void UpdatePos2Entity(ecs::Commands&, ecs::Querier querier, ecs::Resources,
                      ecs::Events&) {
    auto entities = querier.Query<ecs::With<Particle, ecs::Option<Transform>, ecs::Option<NodeTransform>>>();
    for (auto entity : entities) {
        auto& particle = querier.Get<Particle>(entity);
        updateOnePos(entity, querier, particle.pos);
    }
}

void UpdatePos2Particle(ecs::Commands&, ecs::Querier querier, ecs::Resources,
                        ecs::Events&) {
    auto entities = querier.Query<ecs::With<Particle, ecs::Option<Transform>, ecs::Option<NodeTransform>>>();
    for (auto entity : entities) {
        auto& particle = querier.Get<Particle>(entity);

        if (querier.Has<Transform>(entity)) {
            auto& transform = querier.Get<Transform>(entity);
            particle.pos = transform.position;
        }
        if (querier.Has<NodeTransform>(entity)) {
            auto& transform = querier.Get<NodeTransform>(entity);
            particle.pos = transform.globalTransform.position;
        }
    }
}

// FIXME: use collide table or double dispatch to simplify code
bool isShapeCollide(const Shape* const s1, const Shape* const s2) {
    if (s1->GetType() == ShapeAABB) {
        if (s2->GetType() == ShapeAABB) {
            return IsAABBIntersect(*(AABB*)s1, *(AABB*)s2);
        } else if (s2->GetType() == ShapeCircle) {
            return IsCircleAABBIntersect(*(Circle*)s2, *(AABB*)s1);
        }
    } else if (s1->GetType() == ShapeCircle) {
        if (s2->GetType() == ShapeAABB) {
            return IsCircleAABBIntersect(*(Circle*)s1, *(AABB*)s2);
        } else if (s2->GetType() == ShapeCircle) {
            return IsCircleIntersect(*(Circle*)s1, *(Circle*)s2);
        }
    }
    return false;
}


Manifold GenManifoldCircle(const Circle& c1, const Circle& c2) {
    Manifold m;
    m.contactNum = 1;
    auto dir = c2.center - c1.center;
    m.contacts[0].normal =  math::Normalize(dir);
    m.contacts[0].penetration = c2.radius + c1.radius - dir.Length();
    return m;
}

std::array<math::Vector2, 4> getAABBPoints(const AABB& a) {
    return {
        a.center - a.halfLen,
            a.center + math::Vector2(a.halfLen.x, -a.halfLen.y),
            a.center + a.halfLen,
            a.center + math::Vector2(-a.halfLen.x, a.halfLen.y),
    };
}

Manifold GenManifoldCircleAABB(const Circle& c, const AABB& aabb) {
    Manifold m;
    auto pts = getAABBPoints(aabb);
    auto nearestPt = NearestPtOnAABB(c.center, aabb);
    m.contactNum = 1;
    m.contacts[0].normal = nearestPt - c.center;
    m.contacts[0].penetration = c.radius - m.contacts[0].normal.Length();
    m.contacts[0].normal.Normalize();
    return m;
}

Manifold GenManifoldAABB(const AABB& a, const AABB& b) {
    Manifold m;
    auto pts1 = getAABBPoints(a);
    auto pts2 = getAABBPoints(b);
    for (int i = 0; i < 4; i++) {
        const auto& p11 = pts2[i];
        const auto& p12 = pts2[(i + 1) % 4];
        auto d1 = p12 - p11;

        for (int j = 0; j < 4; j++) {
            if (m.contactNum >= 2) {
                break;
            }

            const auto& p21 = pts1[j];
            const auto& p22 = pts1[(j + 1) % 4];

            auto d2 = p22 - p21;

            auto intersectedPts = IntersectLine(p11, d1, p21, d2);
            if (!intersectedPts) continue;

            auto [t1, t2] = intersectedPts.value();
            if (0 <= t1 && t1 <= 1 && 0 <= t2 && t2 <= 1) {
                Contact contact;
                contact.normal.Set(d2.y, -d2.x);
                contact.normal.Normalize();
                contact.point = p11 + t1 * d1;
                auto centerDir = b.center - a.center;
                auto maxDir = a.halfLen + b.halfLen;
                contact.penetration = std::abs((maxDir - math::Vector2(std::abs(centerDir.x), std::abs(centerDir.y))).Dot(contact.normal)) / 2.0;
                m.contacts[m.contactNum] = contact;
                m.contactNum ++;
            }
        }
    }

    if (m.contactNum == 2 &&
        m.contacts[0].normal == -m.contacts[1].normal) {
        auto centerDir = b.center - a.center;
        auto maxDir = a.halfLen + b.halfLen;
        m.contacts[0].normal = math::Normalize(math::Vector2(-m.contacts[0].normal.y, m.contacts[0].normal.x));
        if (m.contacts[0].normal.Dot(centerDir) < 0) {
            m.contacts[0].normal = -m.contacts[0].normal;
        }
        m.contacts[0].penetration = std::abs((maxDir - math::Vector2(std::abs(centerDir.x), std::abs(centerDir.y))).Dot(m.contacts[0].normal)) / 2.0;
        m.contacts[1].normal = m.contacts[0].normal;
        m.contacts[1].penetration = m.contacts[0].penetration;
    }

    return m;
}

// FIXME: use collide table or double dispatch to simplify code
Manifold doShapeCollide(ecs::Entity e1, ecs::Entity e2, Shape* s1, Shape* s2) {
    Manifold m;

    if (s1->GetType() == ShapeAABB) {
        if (s2->GetType() == ShapeAABB) {
            m = GenManifoldAABB(*(AABB*)s1, *(AABB*)s2);
            m.shape1 = s1;
            m.shape2 = s2;
            m.entity1 = e1;
            m.entity2 = e2;
        } else if (s2->GetType() == ShapeCircle) {
            m = GenManifoldCircleAABB(*(Circle*)s2, *(AABB*)s1);
            m.shape1 = s2;
            m.shape2 = s1;
            m.entity1 = e2;
            m.entity2 = e1;
        }
    } else if (s1->GetType() == ShapeCircle) {
        if (s2->GetType() == ShapeAABB) {
            m = GenManifoldCircleAABB(*(Circle*)s1, *(AABB*)s2);
            m.shape1 = s1;
            m.shape2 = s2;
            m.entity1 = e1;
            m.entity2 = e2;
        } else if (s2->GetType() == ShapeCircle) {
            m = GenManifoldCircle(*(Circle*)s1, *(Circle*)s2);
            m.shape1 = s1;
            m.shape2 = s2;
            m.entity1 = e1;
            m.entity2 = e2;
        }
    }

    return m;
}

// for visual debugger, remove it to DebugSystem later
void renderShape(Renderer& renderer, Shape* shape, const Color& color) {
    renderer.SetDrawColor(color);
    if (shape->GetType() == ShapeAABB) {
        const AABB* aabb = (AABB*)(shape);
        renderer.DrawRect(math::Rect{aabb->center.x - aabb->halfLen.x,
                                        aabb->center.y - aabb->halfLen.y,
                                        aabb->halfLen.x * 2.0f,
                                        aabb->halfLen.y * 2.0f});
    } else if (shape->GetType() == ShapeCircle) {
        const Circle* c = (Circle*)(shape);
        renderer.DrawCircle(c->center, c->radius, 30);
    }
}

void doCollideEachOther(std::vector<Manifold>& resultManifolds, std::vector<ecs::Entity>& entities, std::vector<CollideShape*>& shapes, Renderer& renderer) {
    for (auto& shape : shapes) {
        renderShape(renderer, shape->shape.get(), Color{0, 255, 0});
    }

    for (int i = 0; i < (int)shapes.size() - 1; i++) {
        for (int j = i + 1; j < shapes.size(); j++) {
            if (!isShapeCollide(shapes[i]->shape.get(), shapes[j]->shape.get())) {
                continue;
            }
            auto& shape1 = shapes[i]->shape;
            auto& shape2 = shapes[j]->shape;

            Manifold manifold = doShapeCollide(entities[i], entities[j], shape1.get(), shape2.get());

            if (manifold.contactNum > 0) {
                resultManifolds.push_back(std::move(manifold));
            }
        }
    }
}

//! @brief handle collide on one side particle
//! @param manifold 
//! @param p 
//! @param src is particle is src object(if false, we will reverse face normal)
//! @param portion src object mass ratio
void handleCollideToDyn(Manifold& manifold, math::Vector2& p, const math::Vector2& other, bool src, float ratio) {
    math::Vector2 normal;
    const Contact* contact = nullptr;
    float penetration = 0.0;
    if (manifold.contactNum == 1) {
        contact = &manifold.contacts[0];
    } else {
        contact = manifold.contacts[0].penetration < manifold.contacts[1].penetration? &manifold.contacts[0] : &manifold.contacts[1];
    }

    p += contact->normal * (src ? -1 : 1) * contact->penetration * ratio;
}

void handleCollide(std::vector<Manifold>& manifolds, ecs::Querier querier, Renderer& renderer) {
    // for visual debugger, remove it to DebugSystem later
    for (const auto& manifold : manifolds) {
        renderShape(renderer, manifold.shape1, Color::Red);
        renderShape(renderer, manifold.shape2, Color::Red);

        for (int i = 0; i < manifold.contactNum; i++) {
            const auto& contact = manifold.contacts[i];
            renderer.SetDrawColor(Color::Blue);
            renderer.DrawCircle(contact.point, 5);
            renderer.DrawLine(contact.point, contact.point + contact.penetration * contact.normal);
        }
    }


    for (auto& manifold : manifolds)  {
        if (querier.Has<Particle>(manifold.entity1) && querier.Has<Particle>(manifold.entity2)) {
            auto& p1 = querier.Get<Particle>(manifold.entity1);
            auto& p2 = querier.Get<Particle>(manifold.entity2);

            if (p1.massInv == 0 && p2.massInv == 0) {
                return;
            } else if (p1.massInv != 0 && p2.massInv!= 0) {
                float ratio = p1.massInv / (p1.massInv + p2.massInv);
                handleCollideToDyn(manifold, p1.pos, p2.pos, true, ratio);
                handleCollideToDyn(manifold, p2.pos, p1.pos, false, 1.0 - ratio);
            } else if (p1.massInv != 0) {
                handleCollideToDyn(manifold, p1.pos, p2.pos, true, 1.0);
            } else {
                handleCollideToDyn(manifold, p2.pos, p1.pos, false, 1.0);
            }
        } else if (querier.Has<Particle>(manifold.entity1) && !querier.Has<Particle>(manifold.entity2)) {
            auto& p = querier.Get<Particle>(manifold.entity1);
            auto& c = querier.Get<CollideShape>(manifold.entity2);
            if (p.massInv != 0) {
                handleCollideToDyn(manifold, p.pos, c.shape->center, true, 1.0);
            }
        } else if (!querier.Has<Particle>(manifold.entity1) && querier.Has<Particle>(manifold.entity2)) {
            auto& p = querier.Get<Particle>(manifold.entity2);
            auto& c = querier.Get<CollideShape>(manifold.entity1);
            if (p.massInv != 0) {
                handleCollideToDyn(manifold, p.pos, c.shape->center, false, 1.0);
            }
        }
    }
}

void doCollideSystem(ecs::Querier querier, ecs::Resources res) {
    auto entities = querier.Query<CollideShape>();
    std::vector<CollideShape*> shapes;
    for (auto entity : entities) {
        auto& shape = querier.Get<CollideShape>(entity);
        if (querier.Has<Particle>(entity)) {
            shape.shape->center = shape.shape->offset + querier.Get<Particle>(entity).pos;
        } else {
            shape.shape->center = shape.shape->offset;
        }
        shapes.push_back(&shape);
    }

    auto& renderer = res.Get<Renderer>();
    std::vector<Manifold> manifolds;
    doCollideEachOther(manifolds, entities, shapes, renderer);
    handleCollide(manifolds, querier, renderer);
}

void DoCollideSystem(ecs::Commands&, ecs::Querier querier, ecs::Resources res,
                     ecs::Events&) {
    doCollideSystem(querier, res);
}

void Grid::Add(ecs::Entity entity, const math::Rect& rect) {
    auto [hori, vert] = CalcContainedRange(rect);

    for (int x = hori.first; x < hori.second; x++) {
        for (int y = vert.first; y < vert.second; y++) {
            grid_.Get(x, y).push_back(entity);
        }
    }
}

void Grid::Change(ecs::Entity entity, const math::Rect& oldRect,
                    const math::Rect& newRect) {
    Remove(entity, oldRect);
    Add(entity, newRect);
}

void Grid::Remove(ecs::Entity entity, const math::Rect& rect) {
    auto [hori, vert] = CalcContainedRange(rect);

    for (int x = hori.first; x < hori.second; x++) {
        for (int y = vert.first; y < vert.second; y++) {
            auto& grid = grid_.Get(x, y);
            for (auto& entity : grid) {
                if (entity == entity) {
                    std::swap(entity, grid.back());
                    grid.pop_back();
                    break;
                }
            }
        }
    }
}

std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>
Grid::CalcContainedRange(const math::Rect& rect) {
    return {{std::max<size_t>(rect.x / cellW_, 0),
             std::min<size_t>((rect.x + rect.w) / cellW_, grid_.Width())},
            {std::max<size_t>(rect.y / cellH_, 0),
             std::min<size_t>((rect.y + rect.h) / cellH_, grid_.Height())}
    };
}

}  // namespace physic