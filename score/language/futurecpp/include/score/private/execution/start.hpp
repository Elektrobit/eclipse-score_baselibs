///
/// @file
/// @copyright Copyright (C) 2024, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_START_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_START_HPP

#include <score/private/execution/operation_state_t.hpp>
#include <score/private/utility/static_const.hpp>

#include <type_traits>

namespace score::cpp
{
namespace execution
{

namespace detail
{
namespace start_t_disable_adl
{

struct start_t
{
    template <typename OperationState>
    void operator()(OperationState& op) const
    {
        static_assert(is_operation_state<OperationState>::value, "not an operation state");
        static_assert(!std::is_rvalue_reference<decltype(op)>::value, "ill-formed");
        using return_type = decltype(op.start());
        static_assert(std::is_same<void, return_type>::value, "must return void");

        op.start();
    }
};

} // namespace start_t_disable_adl
} // namespace detail

using detail::start_t_disable_adl::start_t;

namespace // NOLINT(google-build-namespaces)
{

/// \brief Starts the asynchronous operation associated with the operation state object.
///
/// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2300r7.html#spec-execution.opstate.start
///
/// `start_t` denotes a customization point. Define the customization point as a member function with `start_t` as
/// the first argument. Follows https://wg21.link/p2855r1.
constexpr auto& start = score::cpp::static_const<start_t>::value; // NOLINT(misc-definitions-in-headers)

} // namespace

} // namespace execution
} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_START_HPP
