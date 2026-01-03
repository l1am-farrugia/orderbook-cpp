#include "order_book.h"

#include <cassert>

namespace ob
{
    bool OrderBook::add_limit(const Order& order)
    {
        order.validate_or_throw();

        //  invariant
        const bool inserted = live_ids_.insert(order.id).second;
        return inserted;
    }

    bool OrderBook::cancel(OrderId id)
    {
        if (id == 0)
        {
            return false;
        }

        const std::size_t erased = live_ids_.erase(id);
        assert(erased <= 1);
        return erased == 1;
    }

    std::size_t OrderBook::live_order_count() const
    {
        return live_ids_.size();
    }
}
