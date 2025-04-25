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
#ifndef LIB_UTILS_SRC_SCOPED_OPERATION_H
#define LIB_UTILS_SRC_SCOPED_OPERATION_H

#include <score/callback.hpp>

namespace score
{
namespace utils
{

/// @brief A simple wrapper to call a Callable on scope exit (like std::experimental::scope_exit)
///
/// @tparam Capacity The capacity to use by callback in storage
/// @tparam Alignment The alignment of callback in storage
template <std::size_t Capacity = score::cpp::callback<void()>::capacity_t::value,
          std::size_t Alignment = score::cpp::callback<void()>::alignment_t::value>
class ScopedOperation final
{
  private:
    using CallbackType = score::cpp::callback<void(), Capacity, Alignment>;

  public:
    explicit ScopedOperation(CallbackType fn) : fn_{std::move(fn)} {}

    ~ScopedOperation()
    {
        fn_();
    }

    ScopedOperation(const ScopedOperation&) = delete;
    ScopedOperation(ScopedOperation&&) = delete;
    ScopedOperation& operator=(const ScopedOperation&) = delete;
    ScopedOperation& operator=(ScopedOperation&&) = delete;

  private:
    CallbackType fn_;
};

}  // namespace utils
}  // namespace score

#endif  // LIB_UTILS_SRC_SCOPED_OPERATION_H
