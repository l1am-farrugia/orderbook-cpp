#pragma once

#include "order.h"

#include <cstdint>
#include <string>

namespace ob
{
    // event type is the category of externally visible output
    enum class EventType
    {
        OrderAccepted,
        OrderRejected,
        OrderCancelled,
        CancelRejected
    };

    // event is produced by the engine and can be logged or tested
    struct Event
    {
        // what happened
        EventType type { EventType::OrderRejected };

        // id the event refers to
        OrderId id { 0 };

        // seq is assigned by the book on accept and reused for cancel reporting
        std::uint64_t seq { 0 };

        // these fields are meaningful for add related events
        Side side { Side::Buy };
        PriceTicks price_ticks { 0 };
        Qty qty { 0 };

        // reason is a small stable token
        std::string reason;
    };
}
