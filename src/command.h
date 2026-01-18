#pragma once

#include "order.h"

namespace ob
{
    // command category
    enum class CommandType
    {
        AddLimit,
        Cancel
    };

    // command is an input record to the engine
    struct Command
    {
        CommandType type { CommandType::AddLimit };

        // common field
        OrderId id { 0 };

        // add limit fields
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
