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
#include "score/utils/src/scoped_operation.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace score
{
namespace platform
{
namespace ifwi
{
namespace
{

TEST(ScopedOperationTest, callFunctionOnDestruction)
{
    bool functionCalled = false;
    {
        utils::ScopedOperation<> scopedOperation{[&functionCalled]() noexcept {
            functionCalled = true;
        }};
        functionCalled = false;
    }
    ASSERT_TRUE(functionCalled);
}

}  // namespace
}  // namespace ifwi
}  // namespace platform
}  // namespace score
