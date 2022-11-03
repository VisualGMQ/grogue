#pragma once

#include "engine/engine.hpp"

class HandFrameRenderSystem;

namespace component {

class HandFrame: public engine::Component {
public:
    friend class HandFrameRenderSystem;

    HandFrame(engine::ComponentID id): engine::Component(id) {
        if (!engine::ImageFactory::Find("tilesheet#hand_frame", frameImage_)) {
            Loge("image `tilesheet#hand_frame` not found");
        }
        Reset();
    }
    void Reset() {
        engine::Component::Reset();

        entity = nullptr;
        position.Set(0, 0);
    }

    engine::Entity* entity;
    engine::Vec2 position;

private:
    engine::Image frameImage_;
};

}