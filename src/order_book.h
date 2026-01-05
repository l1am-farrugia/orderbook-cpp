#pragma once

#include "order.h"

#include <cstddef>
#include <cstdint>
#include <unordered_map>

namespace ob
{
    // result for add limit requests
    enum class AddResult
    {
        Accepted,
        DuplicateId,
        Invalid
    };

    // result for cancel requests
    enum class CancelResult
    {
        Cancelled,
        NotFound,
        Invalid
    };

    // add outcome includes seq when accepted
    struct AddOutcome
    {
        AddResult result { AddResult::Invalid };
        std::uint64_t seq { 0 };
    };

    // cancel outcome includes the original seq when cancelled
    struct CancelOutcome
    {
        CancelResult result { CancelResult::Invalid };
        std::uint64_t seq { 0 };
    };

    // order book stores live orders and owns sequencing
    class OrderBook
    {
    public:
        // adds a new live limit order if valid and unique
        AddOutcome add_limit(OrderId id, Side side, PriceTicks price_ticks, Qty qty);

        // cancels an existing live order by id
        CancelOutcome cancel(OrderId id);

        // number of live orders currently stored
        std::size_t live_order_count() const;

        // helper used by tests and small demos
        bool has_order(OrderId id) const;

    private:
        // monotonic counter for deterministic sequencing
        std::uint64_t next_seq_ { 1 };

        // live orders indexed by id for fast lookup
        std::unordered_map<OrderId, Order> orders_;
    };
}
