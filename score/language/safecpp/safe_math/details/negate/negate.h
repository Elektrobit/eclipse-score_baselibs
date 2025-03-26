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
#ifndef BASELIBS_SCORE_SAFE_MATH_DETAILS_NEGATE_NEGATE_H
#define BASELIBS_SCORE_SAFE_MATH_DETAILS_NEGATE_NEGATE_H

#include "score/language/safecpp/safe_math/details/absolute/absolute.h"
#include "score/language/safecpp/safe_math/details/cast/cast.h"
#include "score/language/safecpp/safe_math/details/comparison/comparison.h"
#include "score/language/safecpp/safe_math/details/type_traits/type_traits.h"

#include "score/result/result.h"

#include <cstdint>
#include <limits>
#include <type_traits>

namespace score::safe_math
{

// We use this template trick to make an intelligent default choice for the return type while allowing a user to
// specify the return type without having to specify the parameter type.
template <class TempR = void,
          class Unsigned,
          typename std::enable_if_t<is_unsigned_integral<Unsigned>::value, bool> = true,
          class R = prefer_first_type_t<TempR, std::make_signed_t<Unsigned>>,
          typename std::enable_if_t<std::is_integral<R>::value, bool> = true>
// Suppress "UNCAUGHT_EXCEPT" rule findings. This rule states: "Called function throws an exception of type
// std::bad_variant_access".
// Rationale: There is no code path where the exception would be thrown.
// Suppress "AUTOSAR C++14 A15-5-3" rule findings. This rule states: "The std::terminate() function shall not be called
// implicitly.".
// Rationale: There is no code path where std::terminate would be implicitly called.
// coverity[fun_call_w_exception]
// coverity[uncaught_exception]
// coverity[autosar_cpp14_a15_5_3_violation]
score::Result<R> Negate(Unsigned value) noexcept
{
    // Special case for value that is not representable in return type before being negated
    if (std::is_signed<R>::value && CmpEqual(value, Abs(std::numeric_limits<R>::lowest())))
    {
        return std::numeric_limits<R>::lowest();
    }

    // Cast it to a signed representation. If that does not work we can not represent the negated value ever.
    using SignedR = std::make_signed_t<R>;
    const auto expected_cast_value = Cast<SignedR>(value);
    if (!expected_cast_value.has_value())
    {
        return score::MakeUnexpected<R>(expected_cast_value.error());
    }

    // Assumption is that the underlying implementation is using the 2's complement
    using UnsignedR = std::make_unsigned_t<R>;
    static_assert(SignedR{-1} == static_cast<SignedR>(static_cast<UnsignedR>(~(UnsignedR{0U}))),
                  "We expect 2's complement representation.");

    const auto inverse = static_cast<SignedR>(-(expected_cast_value.value()));
    return Cast<R>(inverse);
}

// We use this template trick to make an intelligent default choice for the return type while allowing a user to
// specify the return type without having to specify the parameter type.
template <class TempR = void,
          class Signed,
          typename std::enable_if_t<is_signed_integral<Signed>::value, bool> = true,
          class R = prefer_first_type_t<TempR, std::make_signed_t<Signed>>,
          typename std::enable_if_t<std::is_integral<R>::value, bool> = true>
score::Result<R> Negate(Signed value) noexcept
{
    // LCOV_EXCL_BR_START Tool issue - Branch coverage shows missing coverage for decision, but line coverage proves
    // that all decisions are taken.
    // Take care of negative values
    if (CmpLess(value, 0))
    {
        return Cast<R>(Abs(value));
    }

    // Assumption is that the underlying implementation is using the 2's complement
    using Unsigned = std::make_unsigned_t<Signed>;
    static_assert(Signed{-1} == static_cast<Signed>(static_cast<Unsigned>(~(Unsigned{0U}))),
                  "We expect 2's complement representation.");

    const auto inverse = static_cast<Signed>(-(value));
    return Cast<R>(inverse);
    // LCOV_EXCL_BR_STOP
}

// We use this template trick to make an intelligent default choice for the return type while allowing a user to
// specify the return type without having to specify the parameter type.
template <
    class TempR = void,
    class Floating,
    class R = prefer_first_type_t<TempR, Floating>,
    typename std::enable_if_t<std::is_floating_point<Floating>::value && std::is_floating_point<R>::value, bool> = true>
// Suppress "UNCAUGHT_EXCEPT" rule findings. This rule states: "Called function throws an exception of type
// std::bad_variant_access".
// Rationale: There is no code path where the exception would be thrown.
// Suppress "AUTOSAR C++14 A15-5-3" rule findings. This rule states: "The std::terminate() function shall not be called
// implicitly.".
// Rationale: There is no code path where std::terminate would be implicitly called.
// coverity[fun_call_w_exception]
// coverity[uncaught_exception]
// coverity[autosar_cpp14_a15_5_3_violation]
score::Result<R> Negate(Floating value) noexcept
{
    auto casted_value = Cast<R>(value);
    if (!casted_value.has_value())
    {
        return casted_value;
    }

    // IEEE 754 floating-point types are symmetric. E.g. this operation is always safe.
    static_assert(std::numeric_limits<R>::is_iec559, "Result type must adhere to IEEE 754");
    return -casted_value.value();
}

}  // namespace score::safe_math

#endif  // BASELIBS_SCORE_SAFE_MATH_DETAILS_NEGATE_NEGATE_H
