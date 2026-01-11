#pragma once

#include "order.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

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

    // order book stores live orders grouped by side and price
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

        // best bid price if any bids exist
        std::optional<PriceTicks> best_bid_price() const;

        // best ask price if any asks exist
        std::optional<PriceTicks> best_ask_price() const;

        // ids at a specific level in fifo order
        std::vector<OrderId> order_ids_at(Side side, PriceTicks price_ticks) const;

    private:
        // a price level stores orders in fifo time order
        using PriceLevel = std::list<Order>;

        // a locator points to an order inside the book
        struct Locator
        {
            Side side { Side::Buy };
            PriceTicks price_ticks { 0 };
            PriceLevel::iterator it {};
        };

        // monotonic counter for deterministic sequencing
        std::uint64_t next_seq_ { 1 };

        // bids sorted by best price first
        std::map<PriceTicks, PriceLevel, std::greater<PriceTicks>> bids_;

        // asks sorted by best price first
        std::map<PriceTicks, PriceLevel, std::less<PriceTicks>> asks_;

        // index from id to exact location for fast cancel
        std::unordered_map<OrderId, Locator> index_;
    };
}
