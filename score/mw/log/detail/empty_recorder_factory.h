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
#ifndef SCORE_MW_LOG_DETAIL_EMPTY_RECORDER_FACTORY_H
#define SCORE_MW_LOG_DETAIL_EMPTY_RECORDER_FACTORY_H

#include "score/mw/log/detail/empty_recorder.h"
#include "score/mw/log/detail/log_recorder_factory.hpp"

namespace score
{
namespace mw
{
namespace log
{
namespace detail
{
class EmptyRecorderFactory : public LogRecorderFactory<EmptyRecorderFactory>
{
  public:
    EmptyRecorderFactory() = default;
    explicit EmptyRecorderFactory(score::cpp::pmr::unique_ptr<score::os::Fcntl> fcntl_instance);
    std::unique_ptr<Recorder> CreateConcreteLogRecorder(const Configuration& config,
                                                        score::cpp::pmr::memory_resource* memory_resource);
};

}  // namespace detail
}  // namespace log
}  // namespace mw
}  // namespace score

#endif  // SCORE_MW_LOG_DETAIL_EMPTY_RECORDER_FACTORY_H
