///
/// @file
/// @copyright Copyright (C) 2024, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

#include <score/execution.hpp>
#include <score/execution.hpp> //  // test include guard

#include <gtest/gtest.h>

#include <cstdint>
#include <utility>

namespace score::cpp
{
namespace execution
{
namespace
{

enum class category
{
    const_lvalue,
    const_rvalue,
    lvalue,
    rvalue,
};

struct receiver
{
    using receiver_concept = receiver_t;
};

struct op_state
{
    using operation_state_concept = operation_state_t;

    category sender;
    category receiver;
};

struct sender
{
    using sender_concept = sender_t;

    op_state connect(receiver&) & { return {category::lvalue, category::lvalue}; }
    op_state connect(receiver&&) && { return {category::rvalue, category::rvalue}; }
    op_state connect(const receiver&) const& { return {category::const_lvalue, category::const_lvalue}; }
    op_state connect(const receiver&&) const&& { return {category::const_rvalue, category::const_rvalue}; }
};

TEST(connect_test, connect_GivenRValueSenderAndReceiver_ExpectConnectsSenderAndReceiverInAnOperationState)
{
    sender s{};
    receiver r{};
    const op_state op{connect(std::move(s), std::move(r))};

    EXPECT_EQ(op.sender, category::rvalue);
    EXPECT_EQ(op.receiver, category::rvalue);
}
TEST(connect_test, connect_GivenLValueSenderAndReceiver_ExpectConnectsSenderAndReceiverInAnOperationState)
{
    sender s{};
    receiver r{};
    const op_state op{connect(s, r)};

    EXPECT_EQ(op.sender, category::lvalue);
    EXPECT_EQ(op.receiver, category::lvalue);
}

TEST(connect_test, connect_GivenConstRValueSenderAndReceiver_ExpectConnectsSenderAndReceiverInAnOperationState)
{
    const sender s{};
    const receiver r{};
    const op_state op{connect(std::move(s), std::move(r))};

    EXPECT_EQ(op.sender, category::const_rvalue);
    EXPECT_EQ(op.receiver, category::const_rvalue);
}

TEST(connect_test, connect_GivenConstLValueSenderAndReceiver_ExpectConnectsSenderAndReceiverInAnOperationState)
{
    const sender s{};
    const receiver r{};
    const op_state op{connect(s, r)};

    EXPECT_EQ(op.sender, category::const_lvalue);
    EXPECT_EQ(op.receiver, category::const_lvalue);
}

} // namespace
} // namespace execution
} // namespace score::cpp
