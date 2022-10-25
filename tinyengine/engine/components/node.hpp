#pragma once

#include "engine/core/dllexport.hpp"
#include "engine/ecs/world.hpp"
#include "engine/ecs/component.hpp"
#include "engine/renderer/renderer.hpp"

namespace engine {

void SetNodeParent(Entity* parent, Entity* child);

class DLLEXPORT NodeComponent: public Component {
public:
    NodeComponent(ComponentID id): Component(id) { Reset(); }
    virtual ~NodeComponent() = default;

    virtual void Reset() override {
        Component::Reset();
        for (auto& child : children) {
            World::Instance()->DestroyEntity(child);
        }
        children.clear();
        parentNode_ = nullptr;
    }

    void SetParentNode(Entity* parent) { parentNode_ = parent; }
    Entity* GetParentNode() const { return parentNode_; }

    void Attach(Entity* entity) {
        children.push_back(entity);
        SetNodeParent(Parent(), entity);
    }

    std::vector<Entity*> children;

private:
    Entity* parentNode_ = nullptr;
};

class Node2DComponent: public NodeComponent {
public:
    Node2DComponent(ComponentID id): NodeComponent(id) { Reset(); }
    void Reset() override;

    Vec2 position;
    Vec2 scale;
    float rotation;

    Vec2 globalPosition;
    Vec2 globalScale;
    float globalRotation;
};

class Node2DRoot: public Component {
public:
    Node2DRoot(ComponentID id): Component(id) {}
};

class NodeUIRoot final: public Component {
public:
    NodeUIRoot(ComponentID id): Component(id) {}
};

}