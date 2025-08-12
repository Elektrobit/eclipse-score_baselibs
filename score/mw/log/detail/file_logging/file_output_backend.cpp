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
#include "score/mw/log/detail/file_logging/file_output_backend.h"
#include "score/mw/log/detail/log_record.h"
#include "score/mw/log/slot_handle.h"

namespace score
{
namespace mw
{
namespace log
{
namespace detail
{

FileOutputBackend::FileOutputBackend(std::unique_ptr<IMessageBuilder> message_builder,
                                     const std::int32_t file_descriptor,
                                     std::unique_ptr<CircularAllocator<LogRecord>> allocator,
                                     score::cpp::pmr::unique_ptr<score::os::Fcntl> fcntl_instance,
                                     score::cpp::pmr::unique_ptr<score::os::Unistd> unistd) noexcept
    : Backend(),
      buffer_allocator_(std::move(allocator)),
      slot_drainer_(std::move(message_builder), buffer_allocator_, file_descriptor, std::move(unistd))
{
    const auto flags = fcntl_instance->fcntl(file_descriptor, score::os::Fcntl::Command::kFileGetStatusFlags);
    if (flags.has_value())
    {
        std::ignore = fcntl_instance->fcntl(
            file_descriptor,
            score::os::Fcntl::Command::kFileSetStatusFlags,
            flags.value() | score::os::Fcntl::Open::kNonBlocking | score::os::Fcntl::Open::kCloseOnExec);
    }
}

score::cpp::optional<SlotHandle> FileOutputBackend::ReserveSlot() noexcept
{
    slot_drainer_.Flush();
    const auto slot = buffer_allocator_->AcquireSlotToWrite();

    if (slot.has_value())
    {
        // CircularAllocator has capacity limited by CheckFoxMaxCapacity thus the cast is valid:
        // We intentionally static cast to SlotIndex(uint8_t) to limit memory allocations
        // to the required levels during startup, since there is no need to support slots greater
        // than uint8 as per the current system needs.
        // coverity[autosar_cpp14_a4_7_1_violation]
        return SlotHandle{static_cast<SlotIndex>(slot.value())};
    }
    return {};
}

void FileOutputBackend::FlushSlot(const SlotHandle& slot) noexcept
{
    slot_drainer_.PushBack(slot);
    slot_drainer_.Flush();
}

LogRecord& FileOutputBackend::GetLogRecord(const SlotHandle& slot) noexcept
{
    //  Cast to bigger integer type:
    return buffer_allocator_->GetUnderlyingBufferFor(static_cast<std::size_t>(slot.GetSlotOfSelectedRecorder()));
}

}  // namespace detail
}  // namespace log
}  // namespace mw
}  // namespace score
