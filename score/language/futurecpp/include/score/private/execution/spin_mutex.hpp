///
/// \file
/// \copyright Copyright (C) 2024, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_SPIN_MUTEX_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_SPIN_MUTEX_HPP

#include <score/private/execution/exponential_backoff.hpp>

#include <atomic>

namespace score::cpp
{
namespace execution
{
namespace detail
{

/// \brief A lock that causes a thread trying to acquire it to simply wait in a loop ("spin") while repeatedly checking
/// whether the lock is available.
///
/// Implements https://en.cppreference.com/w/cpp/named_req/Lockable
class spin_mutex
{
public:
    spin_mutex() noexcept : lock_{false} {};
    ~spin_mutex() noexcept = default;

    spin_mutex(const spin_mutex&) = delete;
    spin_mutex& operator=(const spin_mutex&) = delete;
    spin_mutex(spin_mutex&&) = delete;
    spin_mutex& operator=(spin_mutex&&) = delete;

    void lock() noexcept
    {
        exponential_backoff backoff{};

        // use test and test-and-set https://en.wikipedia.org/wiki/Test_and_test-and-set
        while (lock_.exchange(true, std::memory_order_acquire))
        {
            do
            {
                backoff.pause();
            } while (lock_.load(std::memory_order_relaxed));
        }
    }

    bool try_lock() noexcept
    {
        if (lock_.load(std::memory_order_relaxed))
        {
            return false;
        }

        return !lock_.exchange(true, std::memory_order_acquire);
    }

    void unlock() noexcept { lock_.store(false, std::memory_order_release); }

private:
    std::atomic<bool> lock_;
};

} // namespace detail
} // namespace execution
} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_SPIN_MUTEX_HPP
