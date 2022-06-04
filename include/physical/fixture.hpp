#pragma once

#include "collider.hpp"

namespace grogue::physical {

class RigidBody;

class Fixture final {
public:
    static std::shared_ptr<Fixture> Create(RigidBody*, const std::initializer_list<BoxCollider>&);
    static std::shared_ptr<Fixture> Create();

    Fixture(RigidBody*, const std::initializer_list<BoxCollider>&);
    Fixture() = default;

    void AddCollider(const BoxCollider&);

    void ClampOn(RigidBody* body) { rigidBody_ = body; }

    auto begin() { return colliders_.begin(); }
    auto end() { return colliders_.end(); }
    auto begin() const { return colliders_.begin(); }
    auto end() const { return colliders_.end(); }

private:
    RigidBody* rigidBody_ = nullptr;
    std::list<BoxCollider> colliders_;
};

using FixturePtr = std::shared_ptr<Fixture>;

}
