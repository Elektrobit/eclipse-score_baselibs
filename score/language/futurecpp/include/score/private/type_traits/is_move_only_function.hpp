///
/// @file
/// @copyright Copyright (C) 2025, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_TYPE_TRAITS_IS_MOVE_ONLY_FUNCTION_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_TYPE_TRAITS_IS_MOVE_ONLY_FUNCTION_HPP

#include <type_traits>

namespace score::cpp
{

template <class Signature, std::size_t Capacity, std::size_t Alignment>
class move_only_function;

/// \brief Check whether the type is a move_only_function type
/// \tparam T: type to be checked
/// \{
template <typename T>
struct is_move_only_function : std::false_type
{
};

template <class Signature, std::size_t Capacity, std::size_t Alignment>
struct is_move_only_function<move_only_function<Signature, Capacity, Alignment>> : std::true_type
{
};
/// \}

} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_TYPE_TRAITS_IS_MOVE_ONLY_FUNCTION_HPP
