#include "order_book.h"

#include <gtest/gtest.h>

TEST(OrderBook, AcceptsValidOrder)
{
    ob::OrderBook book;

    const auto r = book.add_limit(1, ob::Side::Buy, 10'000, 100);
    EXPECT_EQ(r, ob::AddResult::Accepted);
    EXPECT_EQ(book.live_order_count(), 1u);
    EXPECT_TRUE(book.has_order(1));
}

TEST(OrderBook, RejectsDuplicateId)
{
    ob::OrderBook book;

    EXPECT_EQ(book.add_limit(1, ob::Side::Buy, 10'000, 100), ob::AddResult::Accepted);
    EXPECT_EQ(book.add_limit(1, ob::Side::Sell, 10'001, 50), ob::AddResult::DuplicateId);
    EXPECT_EQ(book.live_order_count(), 1u);
}

TEST(OrderBook, RejectsInvalidInputs)
{
    ob::OrderBook book;

    EXPECT_EQ(book.add_limit(0, ob::Side::Buy, 10'000, 100), ob::AddResult::Invalid);
    EXPECT_EQ(book.add_limit(1, ob::Side::Buy, 0, 100), ob::AddResult::Invalid);
    EXPECT_EQ(book.add_limit(1, ob::Side::Buy, 10'000, 0), ob::AddResult::Invalid);

    EXPECT_EQ(book.live_order_count(), 0u);
}

TEST(OrderBook, CancelSemantics)
{
    ob::OrderBook book;

    EXPECT_EQ(book.cancel(1), ob::CancelResult::NotFound);
    EXPECT_EQ(book.cancel(0), ob::CancelResult::Invalid);

    EXPECT_EQ(book.add_limit(1, ob::Side::Buy, 10'000, 100), ob::AddResult::Accepted);
    EXPECT_EQ(book.cancel(1), ob::CancelResult::Cancelled);
    EXPECT_EQ(book.cancel(1), ob::CancelResult::NotFound);
    EXPECT_EQ(book.live_order_count(), 0u);
}
