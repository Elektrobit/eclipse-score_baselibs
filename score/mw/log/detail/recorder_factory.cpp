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
#include "score/mw/log/detail/recorder_factory.h"

#include "score/mw/log/detail/composite_recorder.h"
#include "score/mw/log/detail/empty_recorder.h"
#include "score/mw/log/detail/error.h"
#include "score/mw/log/detail/initialization_reporter.h"
#include "score/mw/log/detail/recorder_config.h"

#ifdef __QNXNTO__
#include "score/mw/log/detail/slog/slog_backend.h"
#endif

namespace score
{
namespace mw
{
namespace log
{
namespace detail
{

/* KW_SUPPRESS_START: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
/* KW_SUPPRESS_START:AUTOSAR.STYLE.SINGLE_STMT_PER_LINE: false positive */
std::unique_ptr<Recorder> RecorderFactory::CreateRecorderFromLogMode(
    const LogMode& log_mode,
    const Configuration& config,
    score::cpp::pmr::unique_ptr<score::os::Fcntl> fcntl_instance,
    score::cpp::pmr::memory_resource* memory_resource) const noexcept
/* KW_SUPPRESS_END:AUTOSAR.STYLE.SINGLE_STMT_PER_LINE: false positive */
/* KW_SUPPRESS_END: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
{
    if (memory_resource == nullptr)
    {
        ReportInitializationError(score::mw::log::detail::Error::kMemoryResourceError);
        return CreateStub();
    }

    /*
    Deviation from Rule: A2-10-1
    - An identifier declared in an inner scope shall not hide an identifier declared in an outer scope
    Justification:
    - recorder only defined once in this function, no hidden from inner scope
    */
    // coverity[autosar_cpp14_a2_10_1_violation : FALSE]
    std::unique_ptr<Recorder> recorder = std::make_unique<EmptyRecorder>();

    switch (log_mode)
    {
        case LogMode::kRemote:
        {
            RemoteRecorderFactoryType remote_recorder_factory;
            recorder = remote_recorder_factory.CreateLogRecorder(config, memory_resource);
            break;
        }
        case LogMode::kFile:
        {
            FileRecorderFactoryType file_record_factory{std::move(fcntl_instance)};
            recorder = file_record_factory.CreateLogRecorder(config, memory_resource);
            break;
        }
        case LogMode::kConsole:
        {
            ConsoleRecorderFactoryType console_recorder_factory;
            recorder = console_recorder_factory.CreateLogRecorder(config, memory_resource);
            break;
        }
        case LogMode::kSystem:
        {
            SystemRecorderFactoryType system_recorder_factory;
            recorder = system_recorder_factory.CreateLogRecorder(config, memory_resource);
            break;
        }
        case LogMode::kInvalid:  // Intentional fall-through
        {
            ReportInitializationError(Error::kRecorderFactoryUnsupportedLogMode);
            break;
        }
        default:
        {
            ReportInitializationError(Error::kRecorderFactoryUnsupportedLogMode);
            break;
        }
    }

    return recorder;
}

/* KW_SUPPRESS_START: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
/*
Deviation from Rule A15-5-3:
- The std::terminate() function shall not be called implicitly.
Justification:
- std::terminate() could be invoked as this function is noexcept function but result.value() may throw an exception.
- However, we only call result.value() after confirming result.has_value(), so there's no scenario where
std::bad_variant_access can occur.
- Therefore, it's safe to suppress this warning here.
*/
// coverity[autosar_cpp14_a15_5_3_violation]
std::unique_ptr<Recorder> RecorderFactory::CreateFromConfiguration(
    const std::unique_ptr<const ITargetConfigReader> config_reader,
    score::cpp::pmr::memory_resource* memory_resource) const noexcept
/* KW_SUPPRESS_END: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
{
    if (memory_resource == nullptr)
    {
        ReportInitializationError(score::mw::log::detail::Error::kMemoryResourceError);
        return CreateStub();
    }
    const auto result = config_reader->ReadConfig();

    /* KW_SUPPRESS_START: MISRA.LOGIC.NOT_BOOL: False positive: It's correct boolean expression. */
    if (!result.has_value())
    /* KW_SUPPRESS_END: MISRA.LOGIC.NOT_BOOL: False positive: It's correct boolean expression. */
    {
        ReportInitializationError(result.error(), "Failed to load configuration files. Fallback to console logging.");
        return CreateWithConsoleLoggingOnly(memory_resource);
    }

    std::vector<std::unique_ptr<Recorder>> recorders;
    /* KW_SUPPRESS_START:AUTOSAR.LAMBDA.REF_LIFETIME: */
    /* The lambda will be executed within this stack. Thus, all references are still valid */
    std::ignore =
        std::for_each(result->GetLogMode().begin(),
                      result->GetLogMode().end(),
                      [this, &result, &recorders, &memory_resource](const auto& log_mode) {
                          std::ignore = recorders.emplace_back(CreateRecorderFromLogMode(
                              log_mode, result.value(), score::os::Fcntl::Default(memory_resource), memory_resource));
                      });
    /* KW_SUPPRESS_END:AUTOSAR.LAMBDA.REF_LIFETIME */

    if (recorders.empty())
    {
        ReportInitializationError(Error::kNoLogModeSpecified);
        return std::make_unique<EmptyRecorder>();
    }

    if (recorders.size() == 1U)
    {
        return std::move(recorders[0]);
    }

    /* KW_SUPPRESS_START: MISRA.LOGIC.OPERATOR.NOT_BOOL: False positive: No booleans here at all. */
    /* KW_SUPPRESS_START: MISRA.COMMA: False positive: Comma used for constructor arguments. */
    // Composite recorder is needed iff there are more than one activate recorder.
    return std::make_unique<CompositeRecorder>(std::move(recorders));
    /* KW_SUPPRESS_END: MISRA.COMMA: False positive: Comma used for constructor arguments. */
    /* KW_SUPPRESS_END: MISRA.LOGIC.OPERATOR.NOT_BOOL: False positive: No booleans here at all. */
}

/* KW_SUPPRESS_START: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
std::unique_ptr<Recorder> RecorderFactory::CreateWithConsoleLoggingOnly(
    score::cpp::pmr::memory_resource* memory_resource) const noexcept
/* KW_SUPPRESS_END: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
{
    if (memory_resource == nullptr)
    {
        ReportInitializationError(score::mw::log::detail::Error::kMemoryResourceError);
        return CreateStub();
    }

    const Configuration config;  //  using all default values
    ConsoleRecorderFactoryType console_recorder_factory;
    return console_recorder_factory.CreateLogRecorder(config, memory_resource);
}

/* KW_SUPPRESS_START: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
std::unique_ptr<Recorder> RecorderFactory::CreateStub() const noexcept
/* KW_SUPPRESS_END: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
{
    return std::make_unique<EmptyRecorder>();
}

/* KW_SUPPRESS_START: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
std::unique_ptr<score::mw::log::IRecorderFactory> CreateRecorderFactory() noexcept
/* KW_SUPPRESS_END: MISRA.LINKAGE.EXTERN: False positive: It's not declaration. */
{
    return std::make_unique<RecorderFactory>();
}

std::unique_ptr<Recorder> RecorderFactory::CreateFromConfiguration(
    score::cpp::pmr::memory_resource* memory_resource) const noexcept
{
    if (memory_resource == nullptr)
    {
        ReportInitializationError(score::mw::log::detail::Error::kMemoryResourceError);
        //  While we do not have resources to allocate any Recorder we should return nothing:
        return nullptr;
    }

    return CreateFromConfiguration(std::make_unique<TargetConfigReader>(std::make_unique<ConfigurationFileDiscoverer>(
                                       score::os::Path::Default(memory_resource),
                                       score::os::Stdlib::Default(memory_resource),
                                       score::os::Unistd::Default(memory_resource))),
                                   memory_resource);
}

}  // namespace detail
}  // namespace log
}  // namespace mw
}  // namespace score
