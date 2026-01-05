#include "engine.h"

#include <iostream>

static const char* event_type_to_string(ob::EventType t)
{
    // stable strings make outputs easy to diff and test
    switch (t)
    {
    case ob::EventType::OrderAccepted:
        return "order_accepted";
    case ob::EventType::OrderRejected:
        return "order_rejected";
    case ob::EventType::OrderCancelled:
        return "order_cancelled";
    case ob::EventType::CancelRejected:
        return "cancel_rejected";
    }
    return "unknown";
}

static const char* side_to_string(ob::Side s)
{
    return (s == ob::Side::Buy) ? "buy" : "sell";
}

static void print_event(const ob::Event& e)
{
    // prints events as simple key value pairs
    std::cout << event_type_to_string(e.type)
              << " id=" << e.id
              << " seq=" << e.seq
              << " side=" << side_to_string(e.side)
              << " px=" << e.price_ticks
              << " qty=" << e.qty
              << " reason=" << e.reason
              << "\n";
}

int main()
{
    ob::Engine eng;

    // basic demo flow to exercise accepted rejected and cancel paths
    for (const auto& e : eng.apply(ob::Command::add_limit(1, ob::Side::Buy, 10'000, 100)))
    {
        print_event(e);
    }

    for (const auto& e : eng.apply(ob::Command::add_limit(1, ob::Side::Sell, 10'001, 50)))
    {
        print_event(e);
    }

    for (const auto& e : eng.apply(ob::Command::cancel(1)))
    {
        print_event(e);
    }

    for (const auto& e : eng.apply(ob::Command::cancel(1)))
    {
        print_event(e);
    }

    std::cout << "live=" << eng.book().live_order_count() << "\n";
    return 0;
}
