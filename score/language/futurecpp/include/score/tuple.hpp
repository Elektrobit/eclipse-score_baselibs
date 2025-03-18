///
/// \file
/// \copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///
/// \brief Score.Futurecpp.Tuple component
///

#ifndef SCORE_LANGUAGE_FUTURECPP_TUPLE_HPP
#define SCORE_LANGUAGE_FUTURECPP_TUPLE_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace score::cpp
{
namespace detail
{
template <typename T, typename Tuple, std::size_t... I>
constexpr T make_from_tuple_impl(Tuple&& t, const std::index_sequence<I...>)
{
    return T(std::get<I>(std::forward<Tuple>(t))...);
}
} // namespace detail

/// \brief Construct an object of type `T`, using the elements of the tuple `t` as the arguments to the constructor.
///
/// Implements https://isocpp.org/files/papers/N4860.pdf#subsection.20.5.5, 2.
/// Note: The tuple need not be std::tuple, and instead may be anything that supports std::get and std::tuple_size; in
/// particular, std::array and std::pair may be used.
template <typename T, typename Tuple>
constexpr T make_from_tuple(Tuple&& t)
{
    return score::cpp::detail::make_from_tuple_impl<T>(
        std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_TUPLE_HPP
