#pragma once

#include "pch.hpp"

namespace grogue::core {

template <typename Cont>
class EventDispatcher final {
public:
    EventDispatcher(Cont& container): container_(container) {}

    template <Uint32 EventTypeT, typename EventStructT>
    void Dispatch(const EventStructT& e) {
        for (auto& elem : container_) {
            if (!(elem->template Handle<EventTypeT, EventStructT>(e))) {
                break;
            }
        }
    }

private:
    Cont& container_;
};

}
