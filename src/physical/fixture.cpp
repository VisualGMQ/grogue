#include "physical/fixture.hpp"

namespace grogue::physical {

Fixture::Fixture(RigidBody* body, const std::initializer_list<BoxCollider>& colliders)
: rigidBody_(body), colliders_(colliders) {}

void Fixture::AddCollider(const BoxCollider& collider) {
    colliders_.push_back(collider);
}

std::shared_ptr<Fixture> Fixture::Create(RigidBody* body, const std::initializer_list<BoxCollider>& colliders) {
    return std::make_shared<Fixture>(body, colliders);
}

std::shared_ptr<Fixture> Fixture::Create() {
    return std::make_shared<Fixture>();
}

}
