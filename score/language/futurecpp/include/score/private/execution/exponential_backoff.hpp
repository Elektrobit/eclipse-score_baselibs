///
/// \file
/// \copyright Copyright (C) 2024, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_EXPONENTIAL_BACKOFF_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_EXPONENTIAL_BACKOFF_HPP

#include <score/private/thread/this_thread.hpp>

#if defined(__x86_64__)
#include <score/private/execution/cpu_relax_x86_64.hpp>
#elif defined(__aarch64__)
#include <score/private/execution/cpu_relax_aarch64.hpp>
#else
#error "unknown platform"
#endif

#include <cstdint>

namespace score::cpp
{
namespace execution
{
namespace detail
{

/// \brief Strategy to pause the thread with an exponential backoff
///
/// https://en.wikipedia.org/wiki/Exponential_backoff
class exponential_backoff
{
public:
    exponential_backoff() noexcept : count_{1} {}
    ~exponential_backoff() noexcept = default;

    exponential_backoff(const exponential_backoff&) = delete;
    exponential_backoff& operator=(const exponential_backoff&) = delete;
    exponential_backoff(exponential_backoff&&) = delete;
    exponential_backoff& operator=(exponential_backoff&&) = delete;

    void pause() noexcept
    {
        if (count_ <= score::cpp::execution::detail::get_platform_specific_cpu_relaxation_count())
        {
            score::cpp::execution::detail::cpu_relax(count_);
            count_ *= 2;
        }
        else
        {
            score::cpp::this_thread::yield();
        }
    }

private:
    std::int32_t count_;
};

} // namespace detail
} // namespace execution
} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_EXPONENTIAL_BACKOFF_HPP
