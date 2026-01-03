#include "order_book.h"

#include <cassert>

namespace ob
{
    AddResult OrderBook::add_limit(OrderId id, Side side, PriceTicks price_ticks, Qty qty)
    {
        if (!is_valid_input(id, price_ticks, qty))
        {
            return AddResult::Invalid;
        }

        const auto it = orders_.find(id);
        if (it != orders_.end())
        {
            return AddResult::DuplicateId;
        }

        Order o {};
        o.id = id;
        o.side = side;
        o.price_ticks = price_ticks;
        o.qty = qty;
        o.seq = next_seq_;

        ++next_seq_;
        const auto inserted = orders_.emplace(id, o).second;

        // Invariant: we already checked for duplicates.
        assert(inserted);
        return AddResult::Accepted;
    }

    CancelResult OrderBook::cancel(OrderId id)
    {
        if (id == 0)
        {
            return CancelResult::Invalid;
        }

        const std::size_t erased = orders_.erase(id);
        if (erased == 1)
        {
            return CancelResult::Cancelled;
        }

        assert(erased == 0);
        return CancelResult::NotFound;
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
