#include "app/app.hpp"

// physical

enum class ShapeType {
    Unknown = 0,
    Circle,
    AABB,
};

class Shape {
public:
    Shape(ShapeType type) : type_(type) {}
    virtual ~Shape() = default;

    ShapeType GetType() const {
        return type_;
    }

private:
    ShapeType type_;
};

struct ManifoldPoint final {
    math::Vector2 point;
    math::Vector2 norma;
};

struct ManifoldDetail final {
    Time::TimeType collideTime{};
    std::array<ManifoldPoint, 2> contactPoint;
    int contactNum{};
};

struct Manifold final {
    Shape* shape1 = nullptr;
    Shape* shape2 = nullptr;
    ManifoldDetail manifold; // from shape1 to shape2
};

struct Circle : public Shape {
    Circle(const math::Vector2& center, float radius)
        : Shape(ShapeType::Circle), center(center), radius(radius) {}

    math::Vector2 center;
    float radius;
};

struct AABB : public Shape {
    AABB(const math::Vector2& min, float w, float h)
        : Shape(ShapeType::AABB), min(min), max(min + math::Vector2(w, h)) {}

    math::Vector2 min;
    math::Vector2 max;
};

Circle* Cast2Circle(Shape* shape) {
    if (shape->GetType() == ShapeType::Circle) {
        return (Circle*)shape;
    }
    return nullptr;
}

AABB* Cast2AABB(Shape* shape) {
    if (shape->GetType() == ShapeType::AABB) {
        return (AABB*)shape;
    }
    return nullptr;
}

math::Vector2 ClosetPointFromAABB(const math::Vector2& pt, const AABB& aabb) {
    math::Vector2 result;
    float x = pt.x;
    if (x <= aabb.min.x) { x = aabb.min.x; }
    if (x >= aabb.max.x) { x = aabb.max.x; }
    result.x = x;

    float y = pt.y;
    if (y <= aabb.min.y) { y = aabb.min.y; }
    if (y >= aabb.max.y) { y = aabb.max.y; }
    result.y = y;

    return result;
}

bool IsCirclesCollide(const Circle& c1, const Circle& c2) {
    float radiusSum = c1.radius + c2.radius;
    return math::LengthSquare(c1.center - c2.center) < radiusSum * radiusSum;
}

bool IsAABBsCollide(const AABB& a, const AABB& b) {
    return !(a.min.x >= b.max.x || a.min.y >= b.max.y || b.min.x >= a.max.x ||
             b.min.y >= a.max.y);
}

bool IsPointInAABB(const AABB& a, const math::Vector2& b) {
    return a.min.x <= b.x && a.max.x >= b.x && a.min.y <= b.y && a.max.y >= b.y;
}

bool IsCircleAABBCollide(const Circle& c, const AABB& a) {
    return math::LengthSquare(ClosetPointFromAABB(c.center, a)) <
           c.radius * c.radius;
}

math::Vector2 ClosetPointFromCircle(const math::Vector2& pt, const Circle& circle) {
    auto dir = pt - circle.center;
    float dist2 = math::Length(dir);
    dir.Normalize();
    return dist2 > circle.radius * circle.radius ? dir * std::sqrt(dist2)
                                                 : dir * circle.radius;
}

// shape manager

class ShapeManager final {
public:
    Circle* CreateCircle(const math::Vector2& center, float radius) {
        shapes_.emplace_back(std::make_unique<Circle>(center, radius));
        return (Circle*)shapes_.back().get();
    }

    AABB* CreateAABB(const math::Vector2& center, float w, float h) {
        shapes_.emplace_back(std::make_unique<AABB>(center, w, h));
        return (AABB*)shapes_.back().get();
    }
    
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};

void RenderAABB(Renderer& renderer, AABB& aabb, const Color& color) {
    renderer.SetDrawColor(color);
    renderer.DrawRect(math::Rect{aabb.min.x, aabb.min.y,
                                 aabb.max.x - aabb.min.x,
                                 aabb.max.y - aabb.min.y});
}

void RenderCircle(Renderer& renderer, Circle& circle, const Color& color) {
    renderer.SetDrawColor(color);
    renderer.DrawCircle(circle.center, circle.radius);
}

// app

class Physical : public App {
public:
    Physical() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
             .AddSystem(ExitTrigger::DetectExitSystem);
    }

private:
};

RUN_APP(Physical)