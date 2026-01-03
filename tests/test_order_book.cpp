#include "order_book.h"

#include <gtest/gtest.h>

TEST(OrderBook, AddLimitRejectsInvalid)
{
    ob::OrderBook book;

    ob::Order bad {};
    bad.id = 1;
    bad.side = ob::Side::Buy;
    bad.price_ticks = 100;
    bad.qty = 0;   // invalid
    bad.seq = 1;

    EXPECT_THROW(book.add_limit(bad), std::invalid_argument);
}

TEST(OrderBook, AddLimitRejectsDuplicateId)
{
    ob::OrderBook book;

    ob::Order o {};
    o.id = 7;
    o.side = ob::Side::Sell;
    o.price_ticks = 123;
    o.qty = 5;
    o.seq = 1;

    EXPECT_TRUE(book.add_limit(o));
    EXPECT_FALSE(book.add_limit(o));
    EXPECT_EQ(book.live_order_count(), 1u);
}

TEST(OrderBook, CancelWorks)
{
    ob::OrderBook book;

    ob::Order o {};
    o.id = 9;
    o.side = ob::Side::Buy;
    o.price_ticks = 1000;
    o.qty = 10;
    o.seq = 1;

    EXPECT_TRUE(book.add_limit(o));
    EXPECT_TRUE(book.cancel(9));
    EXPECT_FALSE(book.cancel(9));
    EXPECT_EQ(book.live_order_count(), 0u);
}
