#include "engine/components/node.hpp"

namespace engine {

void SetNodeParent(Entity* parent, Entity* child) {
    if (auto node = child->GetComponent<NodeComponent>(); node.IsOk()) {
        node.Unwrap()->SetParentNode(parent);
    } else if (auto node = child->GetComponent<Node2DComponent>(); node.IsOk()) {
        node.Unwrap()->SetParentNode(parent);
    } else {
        Loge("entity {} don't has any node component", child->Name());
    }
}

void Node2DComponent::Reset() {
    NodeComponent::Reset();

    position.Set(0, 0);
    scale.Set(1, 1);
    rotation = 0;

    globalPosition = position;
    globalScale = scale;
    globalRotation = rotation;
} 

}