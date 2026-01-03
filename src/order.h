#pragma once

#include <cstdint>
#include <stdexcept>

namespace ob
{
    using OrderId = std::uint64_t;

    enum class Side
    {
        Buy,
        Sell
    };

    struct Order
    {
        OrderId id {};
        Side side { Side::Buy };
        std::int64_t price_ticks { 0 };
        std::int64_t qty { 0 };
        std::uint64_t seq { 0 }; // Monotonic sequence for time-priority

        void validate_or_throw() const
        {
            if (id == 0)
            {
                throw std::invalid_argument("Order id must be non-zero");
            }
            if (price_ticks <= 0)
            {
                throw std::invalid_argument("Order price_ticks must be > 0");
            }
            if (qty <= 0)
            {
                throw std::invalid_argument("Order qty must be > 0");
            }
            if (seq == 0)
            {
                throw std::invalid_argument("Order seq must be non-zero");
            }
        }
    };
}
