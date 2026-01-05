#include "engine.h"

namespace ob
{
    std::vector<Event> Engine::apply(const Command& cmd)
    {
        std::vector<Event> out;

        // add limit maps to accepted or rejected
        if (cmd.type == CommandType::AddLimit)
        {
            const AddOutcome r = book_.add_limit(cmd.id, cmd.side, cmd.price_ticks, cmd.qty);

            Event e {};
            e.id = cmd.id;

            // include input context for add events
            e.side = cmd.side;
            e.price_ticks = cmd.price_ticks;
            e.qty = cmd.qty;

            if (r.result == AddResult::Accepted)
            {
                e.type = EventType::OrderAccepted;
                e.seq = r.seq;
                e.reason = "accepted";
            }
            else
            {
                e.type = EventType::OrderRejected;

                // choose a stable reason token for logs and tests
                if (r.result == AddResult::DuplicateId)
                {
                    e.reason = "duplicate_id";
                }
                else
                {
                    e.reason = "invalid";
                }
            }

            out.push_back(e);
            return out;
        }

        // cancel maps to cancelled or rejected
        if (cmd.type == CommandType::Cancel)
        {
            const CancelOutcome r = book_.cancel(cmd.id);

            Event e {};
            e.id = cmd.id;

            if (r.result == CancelResult::Cancelled)
            {
                e.type = EventType::OrderCancelled;
                e.seq = r.seq;
                e.reason = "cancelled";
            }
            else
            {
                e.type = EventType::CancelRejected;

                // invalid means the request itself was malformed
                if (r.result == CancelResult::Invalid)
                {
                    e.reason = "invalid";
                }
                else
                {
                    e.reason = "not_found";
                }
            }

            out.push_back(e);
            return out;
        }

        // unknown command yields no events  for now
        return out;
    }

    const OrderBook& Engine::book() const
    {
        return book_;
    }
}
