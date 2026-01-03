#pragma once

#include "order.h"

#include <cstdint>
#include <optional>
#include <unordered_set>

namespace ob
{
    class OrderBook
    {
    public:
        bool add_limit(const Order& order);

        bool cancel(OrderId id);

        std::size_t live_order_count() const;

    private:
        std::unordered_set<OrderId> live_ids_;
    };
}
