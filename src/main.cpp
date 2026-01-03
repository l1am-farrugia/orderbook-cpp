#include "order_book.h"

#include <iostream>

static const char* to_string(ob::AddResult r)
{
    switch (r)
    {
    case ob::AddResult::Accepted:
        return "Accepted";
    case ob::AddResult::DuplicateId:
        return "DuplicateId";
    case ob::AddResult::Invalid:
        return "Invalid";
    }
    return "Unknown";
}

int main()
{
    ob::OrderBook book;

    const ob::AddResult r1 = book.add_limit(1, ob::Side::Buy, 10'000, 100);
    const ob::AddResult r2 = book.add_limit(1, ob::Side::Sell, 10'001, 50); // duplicate id

    std::cout << "add(1) -> " << to_string(r1) << "\n";
    std::cout << "add(1 again) -> " << to_string(r2) << "\n";
    std::cout << "live=" << book.live_order_count() << "\n";

    const ob::CancelResult c1 = book.cancel(1);
    const ob::CancelResult c2 = book.cancel(1);

    std::cout << "cancel(1) -> " << (c1 == ob::CancelResult::Cancelled ? "Cancelled" : "Other") << "\n";
    std::cout << "cancel(1 again) -> " << (c2 == ob::CancelResult::NotFound ? "NotFound" : "Other") << "\n";
    std::cout << "live=" << book.live_order_count() << "\n";

    return 0;
}
