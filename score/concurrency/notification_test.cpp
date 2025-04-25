/********************************************************************************
 * Copyright (c) 2025 Contributors to the Eclipse Foundation
 *
 * See the NOTICE file(s) distributed with this work for additional
 * information regarding copyright ownership.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 ********************************************************************************/
#include "score/concurrency/notification.h"

#include <score/callback.hpp>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <chrono>
#include <future>
#include <thread>

namespace score
{
namespace concurrency
{
namespace
{

TEST(Notification, Waits)
{
    using namespace std::chrono_literals;

    Notification notification{};
    std::promise<void> promise{};
    auto f = promise.get_future();

    std::thread waiting_thread{[&notification, promise = std::move(promise)]() mutable {
        notification.waitWithAbort(score::cpp::stop_token{});
        promise.set_value();
    }};

    ASSERT_EQ(f.wait_for(50ms), std::future_status::timeout);  // To ensure that we are blocking in the thread.
    notification.notify();

    waiting_thread.join();
}

TEST(Notification, WaitGetsAborted)
{
    using namespace std::chrono_literals;

    Notification notification{};
    std::promise<void> promise{};
    score::cpp::stop_source stop{};
    auto f = promise.get_future();

    std::thread waiting_thread{[&notification, promise = std::move(promise), token = stop.get_token()]() mutable {
        notification.waitWithAbort(token);
        promise.set_value();
    }};

    ASSERT_EQ(f.wait_for(50ms), std::future_status::timeout);  // To ensure that we are blocking in the thread.
    stop.request_stop();

    waiting_thread.join();
}

TEST(Notification, WaitDoesNotWaitIfAlreadyAborted)
{
    Notification notification{};
    score::cpp::stop_source stop{};
    stop.request_stop();

    std::thread waiting_thread{[&notification, token = stop.get_token()]() mutable {
        notification.waitWithAbort(token);
    }};

    waiting_thread.join();
}

TEST(Notification, WaitsForNotificationAfterNotifying)
{
    using namespace std::chrono_literals;

    Notification notification{};
    std::promise<void> promise{};
    auto f = promise.get_future();

    std::thread waiting_thread{[&notification, promise = std::move(promise)]() mutable {
        notification.waitWithAbort(score::cpp::stop_token{});
        promise.set_value();
    }};

    ASSERT_EQ(f.wait_for(50ms), std::future_status::timeout);  // To ensure that we are blocking in the thread.
    notification.notify();
    notification.reset();

    std::promise<void> promise2{};
    auto f2 = promise2.get_future();

    std::thread waiting_thread2{[&notification, promise = std::move(promise2)]() mutable {
        notification.waitWithAbort(score::cpp::stop_token{});
        promise.set_value();
    }};

    ASSERT_EQ(f2.wait_for(50ms), std::future_status::timeout);  // To ensure that we are blocking in the thread.
    notification.notify();

    waiting_thread.join();
    waiting_thread2.join();
}

TEST(Notification, WaitsForNotificationWithTimeout)
{
    using namespace std::chrono_literals;

    Notification notification{};
    std::promise<void> promise{};
    auto f = promise.get_future();

    std::thread waiting_thread{[&notification, promise = std::move(promise)]() mutable {
        notification.waitForWithAbort(1h, score::cpp::stop_token{});
        promise.set_value();
    }};

    ASSERT_EQ(f.wait_for(50ms), std::future_status::timeout);  // To ensure that we are blocking in the thread.
    notification.notify();

    waiting_thread.join();
}

TEST(Notification, WaitDoesNotWaitIfAlreadyNotified)
{
    using namespace std::chrono_literals;

    // When creating a notification
    Notification notification{};
    std::promise<void> promise{};
    auto f = promise.get_future();

    // And notifying it immediately
    notification.notify();

    // When calling waitWithAbort on the notification object in another thread
    std::thread waiting_thread{[&notification, promise = std::move(promise)]() mutable {
        notification.waitWithAbort(score::cpp::stop_token{});
        promise.set_value();
    }};

    // Then the waitWithAbort call should unblock immediately
    ASSERT_EQ(f.wait_for(50ms), std::future_status::ready);

    waiting_thread.join();
}

TEST(Notification, WaitsForDoesNotWaitIfAlreadyNotified)
{
    using namespace std::chrono_literals;

    // When creating a notification
    Notification notification{};
    std::promise<void> promise{};
    auto f = promise.get_future();

    // And notifying it immediately
    notification.notify();

    // When calling waitForWithAbort on the notification object in another thread
    std::thread waiting_thread{[&notification, promise = std::move(promise)]() mutable {
        notification.waitForWithAbort(1h, score::cpp::stop_token{});
        promise.set_value();
    }};

    // Then the waitForWithAbort call should unblock immediately
    ASSERT_EQ(f.wait_for(50ms), std::future_status::ready);

    waiting_thread.join();
}

TEST(Notification, WaitsForGetsAborted)
{
    using namespace std::chrono_literals;

    Notification notification{};
    std::promise<void> promise{};
    score::cpp::stop_source stop{};
    auto f = promise.get_future();

    std::thread waiting_thread{[&notification, promise = std::move(promise), token = stop.get_token()]() mutable {
        notification.waitForWithAbort(1h, token);
        promise.set_value();
    }};

    ASSERT_EQ(f.wait_for(50ms), std::future_status::timeout);  // To ensure that we are blocking in the thread.
    stop.request_stop();

    waiting_thread.join();
}

TEST(Notification, WaitsForDoesNotWaitIfAlreadyAborted)
{
    using namespace std::chrono_literals;

    Notification notification{};
    score::cpp::stop_source stop{};
    stop.request_stop();

    std::thread waiting_thread{[&notification, token = stop.get_token()]() mutable {
        notification.waitForWithAbort(1h, token);
    }};

    waiting_thread.join();
}

class InterruptibleConditionVariableMock
{
  public:
    MOCK_METHOD(void, notify_all, ());
    MOCK_METHOD(void, wait, (std::unique_lock<std::mutex>&, score::cpp::stop_token, score::cpp::callback<bool()>));
    MOCK_METHOD(bool,
                wait_for,
                (std::unique_lock<std::mutex>&,
                 score::cpp::stop_token&,
                 const std::chrono::steady_clock::duration&,
                 score::cpp::callback<bool()>));
    MOCK_METHOD(std::cv_status,
                wait_until,
                (std::unique_lock<std::mutex>&,
                 score::cpp::stop_token&,
                 const std::chrono::time_point<std::chrono::steady_clock>&,
                 score::cpp::callback<bool()>));
};

TEST(Notification, NotificationIsDestructedBeforeAllOperationsAreFinished)
{
    std::mutex helper_mtx;
    std::unique_lock<std::mutex> helper_lock{helper_mtx};
    std::condition_variable helper_cv{};
    std::thread waiting_thread{};
    bool wait_unlock{false};
    std::atomic<bool> destructed{false};

    {
        ::testing::NiceMock<InterruptibleConditionVariableMock> mock_cv{};
        EXPECT_CALL(mock_cv, wait_for(::testing::_, ::testing::_, ::testing::_, ::testing::_))
            .WillOnce([&destructed, &wait_unlock, &helper_mtx, &helper_cv](
                          std::unique_lock<std::mutex>&,
                          score::cpp::stop_token&,
                          const std::chrono::steady_clock::duration& duration,
                          score::cpp::callback<bool()>) {
                std::unique_lock<std::mutex> lock{helper_mtx};
                wait_unlock = true;
                lock.unlock();
                helper_cv.notify_all();
                std::this_thread::yield();
                std::this_thread::sleep_for(duration);
                if (destructed)
                {
                    []() {
                        GTEST_FAIL();
                    }();
                }
                return true;
            });
        NotificationBasic<std::mutex, ::testing::NiceMock<InterruptibleConditionVariableMock>&> notification{mock_cv};

        waiting_thread = std::thread{[&notification]() mutable {
            score::cpp::stop_source source{};
            notification.waitForWithAbort(std::chrono::milliseconds{100}, source.get_token());
        }};

        helper_cv.wait(helper_lock, [&wait_unlock]() {
            return wait_unlock;
        });
    }
    destructed = true;
    waiting_thread.join();
}

}  // namespace
}  // namespace concurrency
}  // namespace score
