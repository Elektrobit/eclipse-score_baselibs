///
/// \file
/// \copyright Copyright (C) 2017-2018, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_TYPE_TRAITS_IS_MAPLIKE_CONTAINER_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_TYPE_TRAITS_IS_MAPLIKE_CONTAINER_HPP

#include <type_traits>

namespace score::cpp
{
namespace detail
{

inline constexpr auto is_maplike_container_impl(...) -> std::false_type { return {}; }

template <typename C, typename = typename C::mapped_type>
constexpr auto is_maplike_container_impl(const C&) -> std::true_type
{
    return {};
}
template <typename T>
struct is_maplike_container : decltype(is_maplike_container_impl(std::declval<T>()))
{
};

} // namespace detail
} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_TYPE_TRAITS_IS_MAPLIKE_CONTAINER_HPP
