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
#include "score/mw/log/detail/empty_recorder_factory.h"
#include "score/mw/log/detail/error.h"
#include "score/mw/log/detail/initialization_reporter.h"

namespace score
{
namespace mw
{
namespace log
{
namespace detail
{
EmptyRecorderFactory::EmptyRecorderFactory(score::cpp::pmr::unique_ptr<score::os::Fcntl> fcntl_instance)
    : LogRecorderFactory<EmptyRecorderFactory>()
{
    std::ignore = fcntl_instance;
}

std::unique_ptr<Recorder> EmptyRecorderFactory::CreateConcreteLogRecorder(const Configuration& config,
                                                                          score::cpp::pmr::memory_resource* memory_resource)
{
    std::ignore = config;
    std::ignore = memory_resource;
    ReportInitializationError(Error::kRecorderFactoryUnsupportedLogMode);
    return std::make_unique<EmptyRecorder>();
}
}  // namespace detail

}  // namespace log
}  // namespace mw
}  // namespace score
