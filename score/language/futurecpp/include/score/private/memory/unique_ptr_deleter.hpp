///
/// \file
/// \copyright Copyright (C) 2018-2022, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///
/// \brief Score.Futurecpp.Memory component
///

// IWYU pragma: private

#ifndef SCORE_LANGUAGE_FUTURECPP_PRIVATE_MEMORY_UNIQUE_PTR_DELETER_HPP
#define SCORE_LANGUAGE_FUTURECPP_PRIVATE_MEMORY_UNIQUE_PTR_DELETER_HPP

#include <score/assert.hpp>
#include <score/memory_resource.hpp>

#include <cstddef>

namespace score::cpp
{
namespace pmr
{
namespace detail
{

/// Custom deleter for score::cpp::pmr::unique_ptr.
class unique_ptr_deleter
{
public:
    unique_ptr_deleter() = default;

    explicit unique_ptr_deleter(score::cpp::pmr::memory_resource* const memory_resource,
                                const std::size_t size,
                                const std::size_t alignment) noexcept
        : memory_resource_{memory_resource}, size_{size}, alignment_{alignment}
    {
    }

    unique_ptr_deleter(const unique_ptr_deleter& rhs) = default;
    unique_ptr_deleter& operator=(const unique_ptr_deleter& rhs) = default;
    unique_ptr_deleter(unique_ptr_deleter&& rhs) = default;
    unique_ptr_deleter& operator=(unique_ptr_deleter&& rhs) = default;
    ~unique_ptr_deleter() = default;

    template <typename T>
    void operator()(T* p) const
    {
        SCORE_LANGUAGE_FUTURECPP_PRECONDITION(memory_resource_ != nullptr);
        p->~T();
        memory_resource_->deallocate(p, size_, alignment_);
    }

    score::cpp::pmr::memory_resource* memory_resource() const { return memory_resource_; }

private:
    score::cpp::pmr::memory_resource* memory_resource_{nullptr};
    std::size_t size_{0U};
    std::size_t alignment_{0U};
};

} // namespace detail
} // namespace pmr
} // namespace score::cpp

#endif // SCORE_LANGUAGE_FUTURECPP_PRIVATE_MEMORY_UNIQUE_PTR_DELETER_HPP
