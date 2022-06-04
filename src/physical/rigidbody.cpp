#include "physical/rigidbody.hpp"

namespace grogue::physical {

std::shared_ptr<RigidBody> RigidBody::Create(float mass) {
    return std::make_shared<RigidBody>(mass);
}

RigidBody::RigidBody(float mass) : massInv(1.0 / mass) {}

void RigidBody::AddForce(const core::Vec2& f) {
    force += f;
}

void RigidBody::SetForce(const core::Vec2& f) {
    force = f;
}

void RigidBody::Update(double seconds) {
    updatePhysial(seconds);
    updateFixture();
}

void RigidBody::updatePhysial(double seconds) {
    oldPos = pos;
    acc = force * massInv; 

    pos += vel * seconds + 0.5 * acc * seconds * seconds;
    vel += acc * seconds;

    force.Set(0, 0);
    acc.Set(0, 0);
}

void RigidBody::updateFixture() {
    if (fixture) {
        for (auto it = fixture->begin(); it != fixture->end(); it++) {
            it->pos = pos + it->offset;
        }
    }
}

}
