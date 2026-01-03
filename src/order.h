#pragma once

#include <cstdint>

namespace ob
{
    using OrderId = std::uint64_t;

    enum class Side
    {
        Buy,
        Sell
    };

    using PriceTicks = std::int64_t;
    using Qty = std::int64_t;

    struct Order
    {
        OrderId id {};
        Side side { Side::Buy };
        PriceTicks price_ticks { 0 };
        Qty qty { 0 };
        std::uint64_t seq { 0 }; 
    };

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
