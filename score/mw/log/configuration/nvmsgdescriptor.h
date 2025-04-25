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
#ifndef SCORE_MW_LOG_CONFIG_NVMSGDESCRIPTOR_H_
#define SCORE_MW_LOG_CONFIG_NVMSGDESCRIPTOR_H_

#include "score/mw/log/detail/logging_identifier.h"
#include "score/mw/log/log_level.h"

#include <string>
#include <unordered_map>

/* KW_SUPPRESS_START:AUTOSAR.STYLE.SINGLE_STMT_PER_LINE: false positive*/
namespace score
/* KW_SUPPRESS_END:AUTOSAR.STYLE.SINGLE_STMT_PER_LINE */
{
namespace mw
{
namespace log
{
namespace config
{

class NvMsgDescriptor
{
  private:
    uint32_t id_msg_descriptor_{};
    /* KW_SUPPRESS_START:MISRA.INIT.BRACES: False positive. Constructor calls, not Array initializations */
    score::mw::log::detail::LoggingIdentifier appid_{"NULL"};
    score::mw::log::detail::LoggingIdentifier ctxid_{"NULL"};
    /* KW_SUPPRESS_END:MISRA.INIT.BRACES: False positive. Constructor calls, not Array initializations */
    mw::log::LogLevel logLevel_{};

  public:
    // Constructor
    NvMsgDescriptor() noexcept
        : NvMsgDescriptor(0U,
                          score::mw::log::detail::LoggingIdentifier{"NULL"},
                          score::mw::log::detail::LoggingIdentifier{"NULL"},
                          mw::log::LogLevel::kOff)
    {
    }

    NvMsgDescriptor(uint32_t id,
                    const score::mw::log::detail::LoggingIdentifier& appid,
                    const score::mw::log::detail::LoggingIdentifier& ctxid,
                    mw::log::LogLevel logLevel) noexcept
        : id_msg_descriptor_(id), appid_(appid), ctxid_(ctxid), logLevel_(logLevel)
    {
    }

    ~NvMsgDescriptor() = default;
    NvMsgDescriptor& operator=(const NvMsgDescriptor& other) noexcept = default;
    NvMsgDescriptor& operator=(NvMsgDescriptor&& other) noexcept = default;
    NvMsgDescriptor(const NvMsgDescriptor& other) noexcept = default;
    NvMsgDescriptor(NvMsgDescriptor&& other) noexcept = default;

    // Getter and Setter for id_msg_descriptor_
    uint32_t GetIdMsgDescriptor() const noexcept
    {
        return id_msg_descriptor_;
    }

    void SetIdMsgDescriptor(uint32_t id) noexcept
    {
        id_msg_descriptor_ = id;
    }

    // Getter and Setter for appid_
    const score::mw::log::detail::LoggingIdentifier& GetAppId() const noexcept
    {
        return appid_;
    }

    void SetAppId(const score::mw::log::detail::LoggingIdentifier& appid) noexcept
    {
        appid_ = appid;
    }

    // Getter and Setter for ctxid_
    const score::mw::log::detail::LoggingIdentifier& GetCtxId() const noexcept
    {
        return ctxid_;
    }

    void SetCtxId(const score::mw::log::detail::LoggingIdentifier& ctxid) noexcept
    {
        ctxid_ = ctxid;
    }

    // Getter and Setter for logLevel_
    mw::log::LogLevel GetLogLevel() const noexcept
    {
        return logLevel_;
    }

    void SetLogLevel(mw::log::LogLevel logLevel) noexcept
    {
        logLevel_ = logLevel;
    }
};

}  // namespace config
}  // namespace log
}  // namespace mw
}  // namespace score

#endif  // SCORE_MW_LOG_CONFIG_NVMSGDESCRIPTOR_H_
