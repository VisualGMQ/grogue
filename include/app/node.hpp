#pragma once

#include "core/ecs.hpp"

struct Node {
    std::optional<ecs::Entity> entity;
    std::vector<Node> children;

    Node() = default;
    Node(ecs::Entity e, const std::vector<Node>& children) : entity(e), children(children) {}

    Node& SetEntity(ecs::Entity e) {
        entity = e;
        return *this;
    }

    Node& AddChild(Node& node) {
        children.push_back(node);
        return *this;
    }

    Node& AddChild(Node&& node) {
        children.push_back(std::move(node));
        return *this;
    }
};