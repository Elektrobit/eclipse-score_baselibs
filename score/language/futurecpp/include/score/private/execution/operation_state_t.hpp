///
/// @file
/// @copyright Copyright (C) 2024, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_OPERATION_STATE_T_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_OPERATION_STATE_T_HPP

#include <score/private/type_traits/remove_cvref.hpp>
#include <type_traits>

namespace score::cpp
{
namespace execution
{

struct operation_state_t
{
};

template <typename OpState>
using is_operation_state =
    std::is_same<operation_state_t, typename score::cpp::remove_cvref_t<OpState>::operation_state_concept>;

} // namespace execution
} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_EXECUTION_OPERATION_STATE_T_HPP
