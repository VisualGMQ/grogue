#pragma once

#include "core/mathf.hpp"
#include "fixture.hpp"

namespace grogue::physical {

struct RigidBody final {
    core::Vec2 oldPos;
    core::Vec2 pos;
    core::Vec2 vel;    
    core::Vec2 acc;
    core::Vec2 force;

    float massInv;

    bool isStatic = false;
    FixturePtr fixture = nullptr;

    static std::shared_ptr<RigidBody> Create(float mass);

    RigidBody(float mass);

    void AddForce(const core::Vec2&);
    void SetForce(const core::Vec2&);

    void Update(double seconds);

private:
    void updatePhysial(double seconds);
    void updateFixture();
};

using RigidBodyPtr = std::shared_ptr<RigidBody>;

}
