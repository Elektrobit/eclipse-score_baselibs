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
#include "gtest/gtest.h"
#include "score/os/mocklib/uname_mock.h"

using ::testing::Return;

namespace score
{
namespace os
{

class UnameTest : public ::testing::Test
{
  public:
    UnameTest() {}
    ~UnameTest() override {}

  protected:
    score::os::UnameWrapperMock unameMock;
    virtual void SetUp() override
    {
        score::os::Uname::set_testing_instance(unameMock);
    }
    virtual void TearDown() override {}
};

TEST_F(UnameTest, GetUnameSuccess)
{
    EXPECT_CALL(unameMock, UnameWrapper(::testing::_)).WillOnce([](struct utsname* info) {
        std::cout << "Pointer received: " << info << std::endl;
        return 0;
    });
    score::cpp::optional<score::os::SystemInfo> sysInfo = score::os::Uname::instance().GetUname();
    EXPECT_TRUE(sysInfo.has_value());
}

TEST_F(UnameTest, GetUnameFailure)
{
    EXPECT_CALL(unameMock, UnameWrapper(::testing::_)).WillOnce([](struct utsname* info) {
        std::cout << "Pointer received: " << info << std::endl;
        return -1;
    });
    score::cpp::optional<score::os::SystemInfo> sysInfo = score::os::Uname::instance().GetUname();
    EXPECT_FALSE(sysInfo.has_value());
}

TEST(UnameWrapperTest, Success)
{
    // Prepare the expected behavior
    struct utsname info;
    strcpy(info.sysname, "MockedSystem");
    strcpy(info.nodename, "MockedNode");
    strcpy(info.release, "MockedRelease");
    strcpy(info.version, "MockedVersion");
    strcpy(info.machine, "MockedMachine");
    score::os::UnameImpl obj;
    const auto result = obj.UnameWrapper(&info);
    EXPECT_EQ(result, 0);
}

}  // namespace os
}  // namespace score
