///
/// \file
/// \copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///

#include <score/private/iterator/iterator.hpp>
#include <score/private/iterator/iterator.hpp> // test include guard

#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>

#include <gtest/gtest.h>

namespace
{

TEST(iterator_test, CanConstuctIteratorAtBeginning)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator{data, true};
    EXPECT_EQ(iterator.current(), data.data());
}

TEST(iterator_test, CanConstuctConstIteratorAtBeginning)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> iterator{data, true};
    EXPECT_EQ(iterator.current(), data.data());
}

TEST(iterator_test, CanConstuctIteratorAtEnd)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator{data, false};
    EXPECT_EQ(iterator.current(), data.data() + data.size());
}

TEST(iterator_test, CanConstuctConstIteratorAtEnd)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> iterator{data, false};
    EXPECT_EQ(iterator.current(), data.data() + data.size());
}

TEST(iterator_test, CanCopyConstructIterator)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator_lhs{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator_rhs{iterator_lhs};
    EXPECT_EQ(iterator_lhs, iterator_rhs);
}

TEST(iterator_test, CanCopyConstructConstIterator)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> iterator_lhs{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> iterator_rhs{iterator_lhs};
    EXPECT_EQ(iterator_lhs, iterator_rhs);
}

TEST(iterator_test, CanMoveConstructIterator)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> moved_from_iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> moved_to_iterator{std::move(moved_from_iterator)};
    EXPECT_EQ(moved_to_iterator.current(), data.data());
}

TEST(iterator_test, CanMoveConstructConstIterator)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> moved_from_iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> moved_to_iterator{std::move(moved_from_iterator)};
    EXPECT_EQ(moved_to_iterator.current(), data.data());
}

TEST(iterator_test, CanCopyConstructConstIteratorFromIterator)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> copied_from_iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> copied_to_iterator{copied_from_iterator};
    EXPECT_EQ(copied_to_iterator.current(), data.data());
}

TEST(iterator_test, CanMoveConstructConstIteratorFromIterator)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> moved_from_iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> moved_to_iterator{std::move(moved_from_iterator)};
    EXPECT_EQ(moved_to_iterator.current(), data.data());
}

TEST(iterator_test, CannotConstructIteratorFromConstIterator)
{
    static_assert(!std::is_constructible<score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false>,
                                         score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true>>::value,
                  "It must be impossible to construct an iterator from a const iterator");
}

TEST(iterator_test, CanCompareEqualityOfIterators)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> const_iterator{iterator};
    EXPECT_TRUE(iterator == iterator);
    EXPECT_TRUE(const_iterator == const_iterator);
    EXPECT_TRUE(iterator == const_iterator);
    EXPECT_TRUE(const_iterator == iterator);

    const_iterator++;
    EXPECT_FALSE(iterator == const_iterator);
    EXPECT_FALSE(const_iterator == iterator);
}

TEST(iterator_test, CanCompareInequalityOfIterators)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> const_iterator{iterator};
    EXPECT_FALSE(iterator != iterator);
    EXPECT_FALSE(const_iterator != const_iterator);
    EXPECT_FALSE(iterator != const_iterator);
    EXPECT_FALSE(const_iterator != iterator);

    const_iterator++;
    EXPECT_TRUE(iterator != const_iterator);
    EXPECT_TRUE(const_iterator != iterator);
}

TEST(iterator_test, CanCompareLessOperatorOfIterators)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> const_iterator{iterator};
    EXPECT_FALSE(iterator < iterator);
    EXPECT_FALSE(const_iterator < const_iterator);
    EXPECT_FALSE(iterator < const_iterator);
    EXPECT_FALSE(const_iterator < iterator);

    const_iterator++;
    EXPECT_TRUE(iterator < const_iterator);

    iterator = score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false>{data, true};
    const_iterator = score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true>{iterator};
    iterator++;
    EXPECT_TRUE(const_iterator < iterator);
}

TEST(iterator_test, CanCompareLessEqualsOperatorOfIterators)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> const_iterator{iterator};
    EXPECT_TRUE(iterator <= iterator);
    EXPECT_TRUE(const_iterator <= const_iterator);
    EXPECT_TRUE(iterator <= const_iterator);
    EXPECT_TRUE(const_iterator <= iterator);

    const_iterator++;
    EXPECT_TRUE(iterator <= const_iterator);
    EXPECT_FALSE(const_iterator <= iterator);

    iterator = score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false>{data, true};
    const_iterator = score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true>{iterator};
    iterator++;
    EXPECT_TRUE(const_iterator <= iterator);
    EXPECT_FALSE(iterator <= const_iterator);
}

TEST(iterator_test, CanCompareGreaterOperatorOfIterators)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> const_iterator{iterator};
    EXPECT_FALSE(iterator > iterator);
    EXPECT_FALSE(const_iterator > iterator);
    EXPECT_FALSE(iterator > const_iterator);
    EXPECT_FALSE(const_iterator > iterator);

    iterator++;
    EXPECT_TRUE(iterator > const_iterator);

    iterator = score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false>{data, true};
    const_iterator = score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true>{iterator};
    const_iterator++;
    EXPECT_TRUE(const_iterator > iterator);
}

TEST(iterator_test, CanCompareGreaterEqualsOperatorOfIterators)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> const_iterator{iterator};
    EXPECT_TRUE(iterator >= iterator);
    EXPECT_TRUE(const_iterator >= iterator);
    EXPECT_TRUE(iterator >= const_iterator);
    EXPECT_TRUE(const_iterator >= iterator);

    iterator++;
    EXPECT_TRUE(iterator >= const_iterator);
    EXPECT_FALSE(const_iterator >= iterator);

    iterator = score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false>{data, true};
    const_iterator = score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true>{iterator};
    const_iterator++;
    EXPECT_TRUE(const_iterator >= iterator);
    EXPECT_FALSE(iterator >= const_iterator);
}

int const_nonconst_overload(score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false>) { return 1; }
int const_nonconst_overload(score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true>) { return 2; }

TEST(iterator_test, ConstNonConstOverloadResolutionWorks)
{
    std::array<std::uint8_t, 2> data{0x01, 0x02};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false> iterator{data, true};
    score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true> const_iterator{data, true};
    EXPECT_EQ(const_nonconst_overload(iterator), 1);
    EXPECT_EQ(const_nonconst_overload(const_iterator), 2);
}

TEST(iterator_test, IteratorTraitsProduceExpectedTypes)
{
    using IteratorTraits = std::iterator_traits<score::cpp::detail::iterator<std::array<std::uint8_t, 2>, false>>;
    static_assert(std::is_same<typename IteratorTraits::difference_type, std::ptrdiff_t>::value, "");
    static_assert(std::is_same<typename IteratorTraits::value_type, std::uint8_t>::value, "");
    static_assert(std::is_same<typename IteratorTraits::pointer, std::uint8_t*>::value, "");
    static_assert(std::is_same<typename IteratorTraits::reference, std::uint8_t&>::value, "");
    static_assert(std::is_same<typename IteratorTraits::iterator_category, std::random_access_iterator_tag>::value, "");

    using ConstIteratorTraits = std::iterator_traits<score::cpp::detail::iterator<std::array<std::uint8_t, 2>, true>>;
    static_assert(std::is_same<typename ConstIteratorTraits::difference_type, std::ptrdiff_t>::value, "");
    static_assert(std::is_same<typename ConstIteratorTraits::value_type, std::uint8_t>::value, "");
    static_assert(std::is_same<typename ConstIteratorTraits::pointer, const std::uint8_t*>::value, "");
    static_assert(std::is_same<typename ConstIteratorTraits::reference, const std::uint8_t&>::value, "");
    static_assert(std::is_same<typename ConstIteratorTraits::iterator_category, std::random_access_iterator_tag>::value,
                  "");
}

TEST(iterator_test, iterator_when_mutable_container_non_const_type_mutable_iterator_non_const_iterator)
{
    std::array<std::int32_t, 2> a{42, 72};
    score::cpp::detail::iterator<decltype(a), false> it{a, true};
    static_assert(std::is_same<std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_mutable_container_non_const_type_immutable_iterator_non_const_iterator)
{
    std::array<std::int32_t, 2> a{42, 72};
    const score::cpp::detail::iterator<decltype(a), false> it{a, true};
    static_assert(std::is_same<std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_mutable_container_non_const_type_mutable_iterator_const_iterator)
{
    std::array<std::int32_t, 2> a{42, 72};
    score::cpp::detail::iterator<decltype(a), true> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_mutable_container_non_const_type_immutable_iterator_const_iterator)
{
    std::array<std::int32_t, 2> a{42, 72};
    const score::cpp::detail::iterator<decltype(a), true> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_mutable_container_const_type_mutable_iterator_non_const_iterator)
{
    std::array<const std::int32_t, 2> a{42, 72};
    score::cpp::detail::iterator<decltype(a), false> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_mutable_container_const_type_immutable_iterator_non_const_iterator)
{
    std::array<const std::int32_t, 2> a{42, 72};
    const score::cpp::detail::iterator<decltype(a), false> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_mutable_container_const_type_mutable_iterator_const_iterator)
{
    std::array<const std::int32_t, 2> a{42, 72};
    score::cpp::detail::iterator<decltype(a), true> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_mutable_container_const_type_immutable_iterator_const_iterator)
{
    std::array<const std::int32_t, 2> a{42, 72};
    const score::cpp::detail::iterator<decltype(a), true> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_immutable_container_non_const_type_mutable_iterator_non_const_iterator)
{ // Must not compile
  // const std::array<std::int32_t, 2> a{42, 72};
  // score::cpp::detail::iterator<std::remove_const_t<decltype(a)>, false> it{a, true};
}

TEST(iterator_test, iterator_when_immutable_container_non_const_type_immutable_iterator_non_const_iterator)
{ // Must not compile
  // const std::array<std::int32_t, 2> a{42, 72};
  // const score::cpp::detail::iterator<std::remove_const_t<decltype(a)>, false> it{a, true};
}

TEST(iterator_test, iterator_when_immutable_container_non_const_type_mutable_iterator_const_iterator)
{
    const std::array<std::int32_t, 2> a{42, 72};
    score::cpp::detail::iterator<std::remove_const_t<decltype(a)>, true> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_immutable_container_non_const_type_immutable_iterator_const_iterator)
{
    const std::array<std::int32_t, 2> a{42, 72};
    const score::cpp::detail::iterator<std::remove_const_t<decltype(a)>, true> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_immutable_container_const_type_mutable_iterator_non_const_iterator)
{
    const std::array<const std::int32_t, 2> a{42, 72};
    score::cpp::detail::iterator<std::remove_const_t<decltype(a)>, false> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_immutable_container_const_type_immutable_iterator_non_const_iterator)
{
    const std::array<const std::int32_t, 2> a{42, 72};
    const score::cpp::detail::iterator<std::remove_const_t<decltype(a)>, false> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_immutable_container_const_type_mutable_iterator_const_iterator)
{
    const std::array<const std::int32_t, 2> a{42, 72};
    score::cpp::detail::iterator<std::remove_const_t<decltype(a)>, true> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

TEST(iterator_test, iterator_when_immutable_container_const_type_immutable_iterator_const_iterator)
{
    const std::array<const std::int32_t, 2> a{42, 72};
    const score::cpp::detail::iterator<std::remove_const_t<decltype(a)>, true> it{a, true};
    static_assert(std::is_same<const std::int32_t&, decltype(*it)>::value, "Failed.");
}

} // namespace
