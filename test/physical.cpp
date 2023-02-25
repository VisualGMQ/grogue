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
    math::Vector2 normal;
};

struct ManifoldDetail final {
    Time::TimeType collideTime{};
    std::array<ManifoldPoint, 2> contactPoint;
    int contactNum{};

    void AddContactPoint(const ManifoldPoint& p) {
        assert(contactNum <= 2);
        contactPoint[contactNum++] = p;
    }
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
    return math::LengthSquare(c.center - ClosetPointFromAABB(c.center, a)) <
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

using ShapeList = std::vector<Shape*>;

void StartupSystem(ecs::Commands& cmds, ecs::Resources resources) {
    cmds.SetResource<ShapeManager>({})
        .SetResource<ShapeList>({});
}

void GenerateShapesSystem(ecs::Commands& cmds, ecs::Resources resources) {
    auto& manager = resources.Get<ShapeManager>();
    auto& shapes = resources.Get<ShapeList>();

    shapes.push_back(manager.CreateAABB({200, 200}, 50, 25));
    shapes.push_back(manager.CreateCircle({0, 0}, 50));
}


void UpdateSystem(ecs::Commands& cmds, ecs::Queryer, ecs::Resources resources, ecs::Events& events) {
    auto& shapeManager = resources.Get<ShapeManager>();
    auto& shapes = resources.Get<ShapeList>();
    auto& renderer = resources.Get<Renderer>();
    auto& mouse = resources.Get<Mouse>();

    AABB* aabb = Cast2AABB(shapes[0]);
    Circle* circle = Cast2Circle(shapes[1]);

    circle->center = mouse.Position();

    Color color = {0, 255, 0};


    auto point = ClosetPointFromAABB(circle->center, *aabb);
        
    Manifold manifold;
    if (IsCircleAABBCollide(*circle, *aabb)) {
        color = {255, 0, 0};
        manifold.shape1 = circle;
        manifold.shape2 = aabb;
        manifold.manifold.collideTime = 0;
        if (point.x == aabb->min.x) {
            ManifoldPoint maniPoint;
            maniPoint.normal = {-1, 0};
            maniPoint.point = point;
            manifold.manifold.AddContactPoint(maniPoint);
        }
        if (point.x == aabb->max.x) {
            ManifoldPoint maniPoint;
            maniPoint.normal = {1, 0};
            maniPoint.point = point;
            manifold.manifold.AddContactPoint(maniPoint);
        }
        if (point.y == aabb->min.y) {
            ManifoldPoint maniPoint;
            maniPoint.normal = {0, -1};
            maniPoint.point = point;
            manifold.manifold.AddContactPoint(maniPoint);
        }
        if (point.y == aabb->max.y) {
            ManifoldPoint maniPoint;
            maniPoint.normal = {0, 1};
            maniPoint.point = point;
            manifold.manifold.AddContactPoint(maniPoint);
        }
    }

    auto center = circle->center;
    for (int i = 0; i < manifold.manifold.contactNum; i++) {
        auto& contact = manifold.manifold.contactPoint[i];
        auto dir = center - contact.point;
        auto norm = math::Normalize(dir);
        auto offset = std::abs(Dot((circle->radius - math::Length(dir)) * norm, contact.normal)) * contact.normal;
        circle->center += offset;

        LOGD(offset);
    }

    renderer.SetDrawColor({255, 255, 0});
    renderer.DrawRect(math::Rect{point.x - 2, point.y - 2, 4, 4});

    for (auto shape : shapes) {
        switch (shape->GetType()) {
            case ShapeType::AABB:
                RenderAABB(renderer, *(Cast2AABB(shape)), color);
                break;
            case ShapeType::Circle:
                RenderCircle(renderer, *(Cast2Circle(shape)), color);
                break;
        }
    }
}


// app

class Physical : public App {
public:
    Physical() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
             .AddStartupSystem(StartupSystem)
             .AddStartupSystem(GenerateShapesSystem)
             .AddSystem(UpdateSystem)
             .AddSystem(ExitTrigger::DetectExitSystem);
    }

private:
};

RUN_APP(Physical)