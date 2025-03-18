///
/// @file
/// @copyright Copyright (C) 2024, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_SET_VALUE_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_SET_VALUE_HPP

#include <score/private/execution/receiver_t.hpp>
#include <score/private/utility/static_const.hpp>

#include <type_traits>
#include <utility>

namespace score::cpp
{
namespace execution
{

namespace detail
{
namespace set_value_t_disable_adl
{

struct set_value_t
{
    template <typename Receiver, typename... Ts>
    void operator()(Receiver&& r, Ts&&... v) const
    {
        static_assert(is_receiver<Receiver>::value, "not a receiver");
        constexpr bool is_const_rvalue{std::is_rvalue_reference<decltype(r)>::value &&
                                       std::is_const<std::remove_reference_t<decltype(r)>>::value};
        static_assert(!(std::is_lvalue_reference<decltype(r)>::value || is_const_rvalue), "ill-formed");
        using return_type = decltype(std::forward<Receiver>(r).set_value(std::forward<Ts>(v)...));
        static_assert(std::is_same<void, return_type>::value, "must return void");

        std::forward<Receiver>(r).set_value(std::forward<Ts>(v)...);
    }
};

} // namespace set_value_t_disable_adl
} // namespace detail

using detail::set_value_t_disable_adl::set_value_t;

namespace // NOLINT(google-build-namespaces)
{

/// \brief Calls the completion function `set_value` on the receiver.
///
/// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2300r7.html#spec-execution.receivers.set_value
///
/// `set_value_t` denotes a customization point. Define the customization point as a member function with `set_value_t`
/// as the first argument. Follows https://wg21.link/p2855r1.
constexpr auto& set_value = score::cpp::static_const<set_value_t>::value; // NOLINT(misc-definitions-in-headers)

} // namespace

} // namespace execution
} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_SET_VALUE_HPP
