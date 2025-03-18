///
/// @file
/// @copyright Copyright (C) 2024, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_SCHEDULE_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_SCHEDULE_HPP

#include <score/private/execution/scheduler_t.hpp>
#include <score/private/execution/sender_t.hpp>
#include <score/private/utility/static_const.hpp>

#include <utility>

namespace score::cpp
{
namespace execution
{

namespace detail
{
namespace schedule_t_disable_adl
{

struct schedule_t
{
    template <typename Scheduler>
    auto operator()(Scheduler s) const
    {
        static_assert(is_scheduler<Scheduler>::value, "not a scheduler");
        using return_type = decltype(s.schedule());
        static_assert(is_sender<return_type>::value, "not a sender");

        return s.schedule();
    }
};

} // namespace schedule_t_disable_adl
} // namespace detail

using detail::schedule_t_disable_adl::schedule_t;

namespace // NOLINT(google-build-namespaces)
{

/// \brief Returns a sender describing the start of a task graph on the provided scheduler.
///
/// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2300r7.html#design-sender-factory-schedule
///
/// `schedule_t` denotes a customization point. Define the customization point as a member function with `schedule_t` as
/// the first argument. Follows https://wg21.link/p2855r1.
///
/// \return A schedule sender.
constexpr auto& schedule = score::cpp::static_const<schedule_t>::value; // NOLINT(misc-definitions-in-headers)

} // namespace

} // namespace execution
} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_SCHEDULE_HPP
