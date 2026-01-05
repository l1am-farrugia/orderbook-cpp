#pragma once

#include "order.h"

namespace ob
{
    // command type describes the kind of request being applied
    enum class CommandType
    {
        AddLimit,
        Cancel
    };

    // command is a simple input record for the engine
    // it is designed to be easy to build and hard to misuse
    struct Command
    {
        // which command variant this is
        CommandType type { CommandType::AddLimit };

        // id is always relevant
        OrderId id { 0 };

        // fields used by add limit
        Side side { Side::Buy };
        PriceTicks price_ticks { 0 };
        Qty qty { 0 };

        // builds an add limit command
        static Command add_limit(OrderId id, Side side, PriceTicks price_ticks, Qty qty)
        {
            Command c {};
            c.type = CommandType::AddLimit;
            c.id = id;
            c.side = side;
            c.price_ticks = price_ticks;
            c.qty = qty;
            return c;
        }

        // builds a cancel command
        static Command cancel(OrderId id)
        {
            Command c {};
            c.type = CommandType::Cancel;
            c.id = id;
            return c;
        }
    };
}
