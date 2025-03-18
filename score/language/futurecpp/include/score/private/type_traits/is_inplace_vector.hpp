///
/// \file
/// \copyright Copyright (C) 2024, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_TYPE_TRAITS_IS_INPLACE_VECTOR_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_TYPE_TRAITS_IS_INPLACE_VECTOR_HPP

#include <type_traits>

namespace score::cpp
{

template <typename T, std::size_t MaxSize>
class inplace_vector;

/// \brief Check whether the type is a inplace_vector type
///
/// \tparam T The type of the elements
template <typename T>
struct is_inplace_vector : std::false_type
{
};

/// \brief Check whether the type is a inplace_vector type
///
/// \tparam T The type of the elements
/// \tparam MaxSize the number of elements in the sequence
template <typename T, std::size_t MaxSize>
struct is_inplace_vector<inplace_vector<T, MaxSize>> : std::true_type
{
};

} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_TYPE_TRAITS_IS_INPLACE_VECTOR_HPP
