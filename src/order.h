#pragma once

#include <cstdint>

namespace ob
{
    // id type for orders
    using OrderId = std::uint64_t;

    // order side
    enum class Side
    {
        Buy,
        Sell
    };

    // price is represented in integer ticks
    using PriceTicks = std::int64_t;

    // quantity is represented as an integer
    using Qty = std::int64_t;

    // order is stored as internal book state
    struct Order
    {
        OrderId id {};
        Side side { Side::Buy };
        PriceTicks price_ticks { 0 };
        Qty qty { 0 };

        // seq is assigned by the book to enforce deterministic ordering
        std::uint64_t seq { 0 };
    };

    // validates caller supplied values for add limit
    inline bool is_valid_input(OrderId id, PriceTicks price_ticks, Qty qty)
    {
        if (id == 0)
        {
            return false;
        }
        if (price_ticks <= 0)
        {
            return false;
        }
        if (qty <= 0)
        {
            return false;
        }
        return true;
    }
}
