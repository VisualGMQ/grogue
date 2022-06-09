#include "game/gameobject.hpp"

namespace grogue {

void GameObject::Destroy() {
    for (auto& pair : components_) {
        auto& list = pair.second;
        list.clear();
    }
}

}
