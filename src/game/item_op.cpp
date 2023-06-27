#include "game/item_op.hpp"

bool ItemOperationManager::HasOperation(std::string_view name) const {
    // FIXME: in find() function, convert std::string_view to std::string will cause memory allocate. I don't want to allcate!
    if (auto it = datas_.find(std::string(name)); it != datas_.end()) {
        return true;
    } else {
        return false;
    }
}

ItemOperationManager::ItemOperationManager() {
    datas_["use"] = Use;
    datas_["drop"] = Drop;
}

bool Use(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&) {
    // TODO: not finish
    return false;
}

bool Drop(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&) {
    // TODO: not finish
    return false;
}