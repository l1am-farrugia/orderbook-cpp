#include "order_book.h"

#include <iostream>

int main()
{
    ob::OrderBook book;

    ob::Order o {};
    o.id = 1;
    o.side = ob::Side::Buy;
    o.price_ticks = 10'000;
    o.qty = 100;
    o.seq = 1;

    const bool ok = book.add_limit(o);

    std::cout << "added=" << (ok ? "true" : "false")
              << " live=" << book.live_order_count() << "\n";

    return 0;
}
