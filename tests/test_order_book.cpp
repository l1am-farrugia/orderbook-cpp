#include "engine.h"

#include <gtest/gtest.h>

TEST(Engine, AssignsMonotonicSeq)
{
    // verify sequencing is owned by the book not the caller
    ob::Engine eng;

    const auto e1 = eng.apply(ob::Command::add_limit(1, ob::Side::Buy, 10'000, 100));
    const auto e2 = eng.apply(ob::Command::add_limit(2, ob::Side::Buy, 10'000, 100));

    ASSERT_EQ(e1.size(), 1u);
    ASSERT_EQ(e2.size(), 1u);

    EXPECT_EQ(e1[0].type, ob::EventType::OrderAccepted);
    EXPECT_EQ(e2[0].type, ob::EventType::OrderAccepted);

    EXPECT_EQ(e1[0].seq, 1u);
    EXPECT_EQ(e2[0].seq, 2u);
}

TEST(Engine, RejectsDuplicateId)
{
    // duplicate ids should be rejected consistently
    ob::Engine eng;

    const auto e1 = eng.apply(ob::Command::add_limit(1, ob::Side::Buy, 10'000, 100));
    const auto e2 = eng.apply(ob::Command::add_limit(1, ob::Side::Sell, 10'001, 50));

    ASSERT_EQ(e1.size(), 1u);
    ASSERT_EQ(e2.size(), 1u);

    EXPECT_EQ(e1[0].type, ob::EventType::OrderAccepted);
    EXPECT_EQ(e2[0].type, ob::EventType::OrderRejected);
    EXPECT_EQ(e2[0].reason, "duplicate_id");
}

TEST(Engine, CancelReturnsOriginalSeq)
{
    // cancel should report the original seq from acceptance
    ob::Engine eng;

    const auto add = eng.apply(ob::Command::add_limit(7, ob::Side::Buy, 10'000, 100));
    ASSERT_EQ(add.size(), 1u);
    ASSERT_EQ(add[0].type, ob::EventType::OrderAccepted);

    const std::uint64_t seq = add[0].seq;

    const auto c1 = eng.apply(ob::Command::cancel(7));
    ASSERT_EQ(c1.size(), 1u);
    EXPECT_EQ(c1[0].type, ob::EventType::OrderCancelled);
    EXPECT_EQ(c1[0].seq, seq);

    const auto c2 = eng.apply(ob::Command::cancel(7));
    ASSERT_EQ(c2.size(), 1u);
    EXPECT_EQ(c2[0].type, ob::EventType::CancelRejected);
    EXPECT_EQ(c2[0].reason, "not_found");
}

TEST(Engine, RejectsInvalidInputs)
{
    // invalid inputs should reject with a stable reason
    ob::Engine eng;

    const auto e1 = eng.apply(ob::Command::add_limit(0, ob::Side::Buy, 10'000, 100));
    const auto e2 = eng.apply(ob::Command::add_limit(1, ob::Side::Buy, 0, 100));
    const auto e3 = eng.apply(ob::Command::add_limit(1, ob::Side::Buy, 10'000, 0));

    ASSERT_EQ(e1.size(), 1u);
    ASSERT_EQ(e2.size(), 1u);
    ASSERT_EQ(e3.size(), 1u);

    EXPECT_EQ(e1[0].type, ob::EventType::OrderRejected);
    EXPECT_EQ(e2[0].type, ob::EventType::OrderRejected);
    EXPECT_EQ(e3[0].type, ob::EventType::OrderRejected);

    EXPECT_EQ(e1[0].reason, "invalid");
    EXPECT_EQ(e2[0].reason, "invalid");
    EXPECT_EQ(e3[0].reason, "invalid");
}
