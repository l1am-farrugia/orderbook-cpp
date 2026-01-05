#include "order_book.h"

#include <cassert>

namespace ob
{
    AddOutcome OrderBook::add_limit(OrderId id, Side side, PriceTicks price_ticks, Qty qty)
    {
        AddOutcome out {}; // result container for caller

        // validate the raw input fields first
        if (!is_valid_input(id, price_ticks, qty))
        {
            out.result = AddResult::Invalid;
            return out;
        }

        // id must be unique among live orders
        if (orders_.find(id) != orders_.end())
        {
            out.result = AddResult::DuplicateId;
            return out;
        }

        // store the order and assign seq from the book
        Order o {};
        o.id = id;
        o.side = side;
        o.price_ticks = price_ticks;
        o.qty = qty;

        // seq is deterministic and monotonic
        o.seq = next_seq_;

        ++next_seq_; // bump the counter

        const bool inserted = orders_.emplace(id, o).second;
        assert(inserted); // inserton should never fail here

        out.result = AddResult::Accepted;
        out.seq = o.seq;
        return out;
    }

    CancelOutcome OrderBook::cancel(OrderId id)
    {
        CancelOutcome out {}; // returned to caller

        // id zero is not a valid cancel request
        if (id == 0)
        {
            out.result = CancelResult::Invalid;
            return out;
        }

        const auto it = orders_.find(id);
        if (it == orders_.end())
        {
            out.result = CancelResult::NotFound;
            return out;
        }

        // keep the original seq so cancel reporting is stable
        out.seq = it->second.seq;

        const std::size_t erased = orders_.erase(id);
        assert(erased == 1); // should erase exactly one element.

        out.result = CancelResult::Cancelled;
        return out;
    }

    std::size_t OrderBook::live_order_count() const
    {
        return orders_.size();
    }

    bool OrderBook::has_order(OrderId id) const
    {
        return orders_.find(id) != orders_.end();
    }
}
