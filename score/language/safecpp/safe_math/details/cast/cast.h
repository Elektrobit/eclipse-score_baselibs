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
#ifndef BASELIBS_SCORE_SAFE_MATH_DETAILS_CAST_CAST_H
#define BASELIBS_SCORE_SAFE_MATH_DETAILS_CAST_CAST_H

#include "score/language/safecpp/safe_math/details/comparison/comparison.h"
#include "score/language/safecpp/safe_math/error.h"

#include "score/result/result.h"

#include <score/math.hpp>

#include <cfenv>
#include <iostream>
#include <limits>

namespace score::safe_math
{
namespace details
{

template <class R,
          class T,
          typename std::enable_if_t<(std::is_integral<T>::value || std::is_floating_point<T>::value) &&
                                        (std::is_integral<R>::value || std::is_floating_point<R>::value),
                                    bool> = true>
constexpr bool IsOutsideBoundsOfR(const T value) noexcept
{
    constexpr auto min = std::numeric_limits<R>::lowest();
    constexpr auto max = std::numeric_limits<R>::max();
    return CmpLess(value, min) || CmpGreater(value, max);
}

template <class R,
          class T,
          typename std::enable_if_t<(std::is_integral<T>::value || std::is_floating_point<T>::value) &&
                                        (std::is_integral<R>::value || std::is_floating_point<R>::value),
                                    bool> = true>
constexpr score::Result<R> CastWithBoundsCheck(T value) noexcept
{
    if (IsOutsideBoundsOfR<R>(value))
    {
        return score::MakeUnexpected(ErrorCode::kExceedsNumericLimits);
    }

    // Suppress of "AUTOSAR C++14 A4-7-1", the rule states: "An integer expression shall not lead to data loss."
    // Safe conversion because the bounds check ensures no data loss.
    // coverity[autosar_cpp14_a4_7_1_violation]
    return static_cast<R>(value);
}

}  // namespace details

template <class R,
          class T,
          typename std::enable_if_t<(std::is_integral<T>::value && std::is_integral<R>::value), bool> = true>
constexpr score::Result<R> Cast(T value) noexcept
{
    return details::CastWithBoundsCheck<R>(value);
}

template <
    class R,
    class T,
    typename std::enable_if_t<(std::is_floating_point<T>::value && std::is_floating_point<R>::value), bool> = true>
// Suppress "UNCAUGHT_EXCEPT" rule findings. This rule states: "Called function throws an exception of type
// std::bad_variant_access".
// Rationale: There is no code path where the exception would be thrown.
// Suppress "AUTOSAR C++14 A15-5-3" rule findings. This rule states: "The std::terminate() function shall not be called
// implicitly.".
// Rationale: There is no code path where std::terminate would be implicitly called.
// coverity[fun_call_w_exception]
// coverity[uncaught_exception]
// coverity[autosar_cpp14_a15_5_3_violation]
constexpr score::Result<R> Cast(T value) noexcept
{
    static_assert(std::numeric_limits<R>::is_iec559, "Result type must follow IEEE 754");
    if (score::cpp::isnan(value))
    {
        return std::numeric_limits<R>::quiet_NaN();
    }

    if (score::cpp::isinf(value))
    {
        constexpr auto result_infinity = std::numeric_limits<R>::infinity();
        return score::cpp::signbit(value) ? -result_infinity : result_infinity;
    }

    const auto result = details::CastWithBoundsCheck<R>(value);

    constexpr std::int32_t tolerance{0};
    if (result.has_value() && CmpNotEqual(static_cast<T>(result.value()), value, tolerance))
    {
        return score::MakeUnexpected(ErrorCode::kImplicitRounding);
    }

    return result;
}

template <class R,
          class T,
          typename std::enable_if_t<(std::is_floating_point<T>::value && std::is_integral<R>::value), bool> = true>
// Suppress "UNCAUGHT_EXCEPT" rule findings. This rule states: "Called function throws an exception of type
// std::bad_variant_access".
// Rationale: There is no code path where the exception would be thrown.
// Suppress "AUTOSAR C++14 A15-5-3" rule findings. This rule states: "The std::terminate() function shall not be called
// implicitly.".
// Rationale: There is no code path where std::terminate would be implicitly called.
// coverity[fun_call_w_exception]
// coverity[uncaught_exception]
// coverity[autosar_cpp14_a15_5_3_violation]
constexpr score::Result<R> Cast(T value) noexcept
{
    if (score::cpp::isnan(value))
    {
        return score::MakeUnexpected(ErrorCode::kExceedsNumericLimits);
    }

    const auto result = details::CastWithBoundsCheck<R>(value);

    constexpr std::int32_t tolerance{4};
    if (result.has_value() && CmpNotEqual(result.value(), value, tolerance))
    {
        return score::MakeUnexpected(ErrorCode::kImplicitRounding);
    }

    return result;
}

template <class R,
          class T,
          typename std::enable_if_t<(std::is_integral<T>::value && std::is_floating_point<R>::value), bool> = true>
constexpr score::Result<R> Cast(T value) noexcept
{
    static_assert(std::numeric_limits<R>::is_iec559, "Result type must follow IEEE 754");

    // Suppress "AUTOSAR C++14 M5-0-4" rule finding
    // This rule states: "An implicit integral conversion shall not change the signedness of the underlying type."
    // Rationale: No implicit cast is happening here. FE_ALL_EXCEPT is the bitwise ORed combination of all available
    // FE test macros. Each FE test macro expands to an integer constant expression. (ISO/IEC 9899:2011 7.6 6)
    // coverity[autosar_cpp14_m5_0_4_violation]
    constexpr std::int32_t fe_all_except = FE_ALL_EXCEPT;

    // This branch is only entered when the FPU fails to reset exception flags.
    // We cannot trigger this artificially.
    // Mocking the behavior would only shift this uncovered line somewhere else.
    // LCOV_EXCL_BR_START See comment above
    if (std::feclearexcept(fe_all_except) != 0)
    {
        // LCOV_EXCL_START See comment above if-statement
        return score::MakeUnexpected(ErrorCode::kUnknown);
        // LCOV_EXCL_STOP
    }
    // LCOV_EXCL_BR_STOP

    const auto result = static_cast<R>(value);

    constexpr std::int32_t tolerance{0};
    if ((std::fetestexcept(fe_all_except) != 0) || CmpNotEqual(result, value, tolerance))
    {
        return score::MakeUnexpected(ErrorCode::kImplicitRounding);
    }

    return result;
}

}  // namespace score::safe_math

#endif  // BASELIBS_SCORE_SAFE_MATH_DETAILS_CAST_CAST_H
