#include "engine.h"

#include "event_io.h"

namespace ob
{
    std::vector<Event> Engine::apply(const Command& cmd)
    {
        std::vector<Event> events;

        // dispatch on command type
        if (cmd.type == CommandType::AddLimit)
        {
            events = book_.add_limit(cmd.id, cmd.side, cmd.price_ticks, cmd.qty);
        }
        else
        {
            events = book_.cancel(cmd.id);
        }

        // log if enabled
        if (log_.has_value())
        {
            for (const auto& e : events)
            {
                // write each event on its own line
                (*log_) << event_to_line(e) << "\n";
            }
            log_->flush();
        }

        return events;
    }

    std::vector<Event> Engine::apply_all(const std::vector<Command>& cmds)
    {
        std::vector<Event> out;

        // apply sequentially to preserve determinism
        for (const auto& c : cmds)
        {
            const auto es = apply(c);
            out.insert(out.end(), es.begin(), es.end());
        }

        return out;
    }

    bool Engine::start_event_log(const std::string& path)
    {
        // opens the log file and enables event writing
        std::ofstream f(path, std::ios::out | std::ios::trunc | std::ios::binary);
        if (!f)
        {
            return false;
        }

        log_.emplace(std::move(f));
        return true;
    }

    void Engine::stop_event_log()
    {
        if (log_.has_value())
        {
            log_->flush();
        }
        log_.reset();
    }

    const OrderBook& Engine::book() const
    {
        return book_;
    }
}
