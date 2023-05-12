#include "game/item_op.hpp"

bool ItemOperationManager::HasOperation(std::string_view name) const {
    if (auto it = datas_.find(name); it != datas_.end()) {
        return true;
    } else {
        return false;
    }
}

ItemOperationManager::ItemOperationManager() {
    datas_["use"] = Use;
    datas_["drop"] = Drop;
}