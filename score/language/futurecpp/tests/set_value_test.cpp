///
/// @file
/// @copyright Copyright (C) 2024, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

#include <score/execution.hpp>
#include <score/execution.hpp> //  // test include guard

#include <score/utility.hpp>

#include <gtest/gtest.h>

namespace score::cpp
{
namespace execution
{
namespace
{

struct receiver
{
    using receiver_concept = receiver_t;

    enum class action
    {
        init,
        value,
    };

    void set_value() & = delete;
    void set_value() && { *a = action::value; }
    void set_value() const& = delete;
    void set_value() const&& = delete;

    action* a{};
};

TEST(set_value_test, set_value_GivenRValueReceiver_ExpectSetValueCalled)
{
    receiver::action a{receiver::action::init};
    set_value(receiver{&a});

    EXPECT_EQ(a, receiver::action::value);
}

struct receiver_args
{
    using receiver_concept = receiver_t;

    struct argument
    {
    };

    enum class category
    {
        const_lvalue,
        const_rvalue,
        lvalue,
        rvalue,
    };

    category get(const argument&) { return category::const_lvalue; }
    category get(const argument&&) { return category::const_rvalue; }
    category get(argument&) { return category::lvalue; }
    category get(argument&&) { return category::rvalue; }

    template <typename... Ts>
    void set_value(Ts&&... v)
    {
        *c = {get(std::forward<Ts>(v))...};
    }

    std::vector<category>* c{};
};

TEST(set_value_test, set_value_GivenArgumentsWithDifferentValueCategories_ExpectsArgumentsPerfectlyForwarded)
{
    std::vector<receiver_args::category> c{};

    receiver_args::argument a{};
    set_value(receiver_args{&c}, a, score::cpp::as_const(a), std::move(a), std::move(score::cpp::as_const(a)));

    ASSERT_EQ(4U, c.size());
    EXPECT_EQ(receiver_args::category::lvalue, c[0U]);
    EXPECT_EQ(receiver_args::category::const_lvalue, c[1U]);
    EXPECT_EQ(receiver_args::category::rvalue, c[2U]);
    EXPECT_EQ(receiver_args::category::const_rvalue, c[3U]);
}

} // namespace
} // namespace execution
} // namespace score::cpp
