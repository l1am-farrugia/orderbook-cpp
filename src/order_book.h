#pragma once

#include "order.h"

#include <cstddef>
#include <cstdint>
#include <unordered_map>

namespace ob
{
    enum class AddResult
    {
        Accepted,
        DuplicateId,
        Invalid
    };

    enum class CancelResult
    {
        Cancelled,
        NotFound,
        Invalid
    };

    class OrderBook
    {
    public:
        AddResult add_limit(OrderId id, Side side, PriceTicks price_ticks, Qty qty);

        CancelResult cancel(OrderId id);

        std::size_t live_order_count() const;

        // helper for tests and simple sanity checks
        bool has_order(OrderId id) const;

    private:
        std::uint64_t next_seq_ { 1 };
        std::unordered_map<OrderId, Order> orders_;
    };
}
