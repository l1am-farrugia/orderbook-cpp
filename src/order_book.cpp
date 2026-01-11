#include "order_book.h"

#include <cassert>

namespace ob
{
    AddOutcome OrderBook::add_limit(OrderId id, Side side, PriceTicks price_ticks, Qty qty)
    {
        AddOutcome out {}; // returned to caller

        // validate the raw input fields first
        if (!is_valid_input(id, price_ticks, qty))
        {
            out.result = AddResult::Invalid;
            return out;
        }

        // id must be unique among live orders
        if (index_.find(id) != index_.end())
        {
            out.result = AddResult::DuplicateId;
            return out;
        }

        // build the stored order state
        Order o {};
        o.id = id;
        o.side = side;
        o.price_ticks = price_ticks;
        o.qty = qty;

        // seq is deterministic and monotonic
        o.seq = next_seq_;
        ++next_seq_;

        // choose the correct side map
        if (side == Side::Buy)
        {
            auto [lvl_it, created] = bids_.try_emplace(price_ticks, PriceLevel {});
            PriceLevel& level = lvl_it->second;

            // append to preserve fifo order at this price
            level.push_back(o);
            auto it = std::prev(level.end());

            const bool inserted = index_.emplace(id, Locator { side, price_ticks, it }).second;
            assert(inserted); // inserton should never fail here

            out.result = AddResult::Accepted;
            out.seq = o.seq;
            return out;
        }
        else
        {
            auto [lvl_it, created] = asks_.try_emplace(price_ticks, PriceLevel {});
            PriceLevel& level = lvl_it->second;

            // append to preserve fifo order at this price
            level.push_back(o);
            auto it = std::prev(level.end());

            const bool inserted = index_.emplace(id, Locator { side, price_ticks, it }).second;
            assert(inserted); // should never fail after duplicate check

            out.result = AddResult::Accepted;
            out.seq = o.seq;
            return out;
        }
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

        auto idx_it = index_.find(id);
        if (idx_it == index_.end())
        {
            out.result = CancelResult::NotFound;
            return out;
        }

        // copy locator data then remove from index at the end
        const Locator loc = idx_it->second;

        // capture original seq so reporting is stable
        out.seq = loc.it->seq;

        if (loc.side == Side::Buy)
        {
            auto lvl_it = bids_.find(loc.price_ticks);
            assert(lvl_it != bids_.end()); // internal book state must exist

            PriceLevel& level = lvl_it->second;

            // erase the exact order by iterator
            level.erase(loc.it);

            // remove empty levels so best price queries stay clean
            if (level.empty())
            {
                bids_.erase(lvl_it);
            }
        }
        else
        {
            auto lvl_it = asks_.find(loc.price_ticks);
            assert(lvl_it != asks_.end()); // internal state should exist

            PriceLevel& level = lvl_it->second;

            // erase the exact order by iterator
            level.erase(loc.it);

            if (level.empty())
            {
                asks_.erase(lvl_it);
            }
        }

        index_.erase(idx_it);

        out.result = CancelResult::Cancelled;
        return out;
    }

    std::size_t OrderBook::live_order_count() const
    {
        return index_.size();
    }

    bool OrderBook::has_order(OrderId id) const
    {
        return index_.find(id) != index_.end();
    }

    std::optional<PriceTicks> OrderBook::best_bid_price() const
    {
        // return best bid if any bids exist
        if (bids_.empty())
        {
            return std::nullopt;
        }
        return bids_.begin()->first;
    }

    std::optional<PriceTicks> OrderBook::best_ask_price() const
    {
        // return best ask if any asks exist
        if (asks_.empty())
        {
            return std::nullopt;
        }
        return asks_.begin()->first;
    }

    std::vector<OrderId> OrderBook::order_ids_at(Side side, PriceTicks price_ticks) const
    {
        // returns ids in fifo order for a given level
        std::vector<OrderId> out_ids;

        if (side == Side::Buy)
        {
            auto it = bids_.find(price_ticks);
            if (it == bids_.end())
            {
                return out_ids;
            }

            const PriceLevel& level = it->second;
            out_ids.reserve(level.size());

            for (const auto& o : level)
            {
                out_ids.push_back(o.id);
            }
            return out_ids;
        }
        else
        {
            auto it = asks_.find(price_ticks);
            if (it == asks_.end())
            {
                return out_ids;
            }

            const PriceLevel& level = it->second;
            out_ids.reserve(level.size());

            for (const auto& o : level)
            {
                out_ids.push_back(o.id);
            }
            return out_ids;
        }
    }
}
