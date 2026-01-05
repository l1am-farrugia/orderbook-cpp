#pragma once

#include "command.h"
#include "event.h"
#include "order_book.h"

#include <vector>

namespace ob
{
    // engine is the command in event out boundary
    // It owns the book state and keeps sequencing consistent
    class Engine
    {
    public:
        // applies one command to the current state
        // returns the events describing the outcome
        std::vector<Event> apply(const Command& cmd);

        // read only access to the underlying book
        const OrderBook& book() const;

    private:
        // order book state for this engine instance
        OrderBook book_;
    };
}
