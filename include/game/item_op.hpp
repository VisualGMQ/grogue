#pragma once

#include "core/pch.hpp"
#include "game/types.hpp"

//! @addtogroup item-operations
//! @brief some operation that can apply on item
//! @{

//! @brief use a item
bool Use(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&);

//! @brief drop a item
bool Drop(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&);

class ItemOperationManager final: public Singleton<ItemOperationManager, true> {
public:
    bool HasOperation(std::string_view name) const;
    const ItemOpFunc& GetOperation(std::string_view name) const;
    const auto& GetAllOperations() const { return datas_; }

private:
    std::unordered_map<std::string, ItemOpFunc> datas_;

    ItemOperationManager();
};

//! @}