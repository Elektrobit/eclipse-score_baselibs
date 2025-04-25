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
#include "score/os/utils/thread.h"
#include "score/os/pthread.h"

#include <score/jthread.hpp>
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

namespace score
{
namespace os
{
namespace
{

using ::testing::Test;

class ThreadNameTest : public Test
{
  protected:
    void SetUp() override
    {
        Test::SetUp();

        thread_ = std::thread{[this] {
            while (!done_.load())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds{10});
            }
        }};
        jthread_ = score::cpp::jthread([this]() {
            while (!done_.load())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds{10});
            }
        });
    }

    void TearDown() override
    {
        done_ = true;
        thread_.join();
        jthread_.join();
        Test::TearDown();
    }

    std::thread thread_;
    score::cpp::jthread jthread_;

  private:
    std::atomic<bool> done_{false};
};

TEST_F(ThreadNameTest, SetNameSucceeds)
{
    RecordProperty("ParentRequirement", "SCR-4976940");
    RecordProperty("ASIL", "B");
    RecordProperty("TestingTechnique", "T-REQ");

    EXPECT_NO_THROW(set_thread_name(thread_, "foobar"));
    // get name and check equality
    constexpr std::size_t length = 16U;
    char name[length];
    const auto ret = score::os::Pthread::instance().getname_np(thread_.native_handle(), name, length);
    EXPECT_TRUE(ret.has_value());
    EXPECT_EQ(std::string(name), "foobar");
}

TEST_F(ThreadNameTest, JthreadSetNameSucceeds)
{
    RecordProperty("ParentRequirement", "SCR-4976940");
    RecordProperty("ASIL", "B");
    RecordProperty("TestingTechnique", "T-REQ");

    EXPECT_NO_THROW(set_thread_name(jthread_, "foobar"));

    // get name and check equality
    constexpr std::size_t length = 16U;
    char name[length];
    const auto ret = score::os::Pthread::instance().getname_np(jthread_.native_handle(), name, length);
    EXPECT_TRUE(ret.has_value());
    EXPECT_EQ(std::string(name), "foobar");
}

TEST_F(ThreadNameTest, SetNameFails)
{
    const size_t name_length = 200U;
    std::string thread_name(name_length, 'a');
    EXPECT_NO_THROW(set_thread_name(thread_, thread_name));
}

TEST_F(ThreadNameTest, GetNameSucceeds)
{
    RecordProperty("ParentRequirement", "SCR-4977102");
    RecordProperty("ASIL", "B");
    RecordProperty("TestingTechnique", "T-REQ");

    EXPECT_NO_THROW(get_thread_name(thread_));
}

TEST(ThreadAffinityTest, SetAffinitySucceeds)
{
    EXPECT_TRUE(set_thread_affinity(0));
}

}  // namespace
}  // namespace os
}  // namespace score
