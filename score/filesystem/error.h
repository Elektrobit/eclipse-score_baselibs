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
#ifndef SCORE_LIB_FILESYSTEM_ERROR_H
#define SCORE_LIB_FILESYSTEM_ERROR_H

#include "score/result/result.h"

namespace score
{
namespace filesystem
{

enum class ErrorCode : score::result::ErrorCode
{
    kCouldNotRetrieveStatus,
    kFromFileDoesNotExist,
    kCouldNotAccessFileDuringCopy,
    kCopyFailed,
    kCouldNotSetPermissions,
    kFileNotCopied,
    kCouldNotCreateDirectory,
    kCouldNotChangePermissions,
    kFileOrDirectoryDoesNotExist,
    kCouldNotRemoveFileOrDirectory,
    kCouldNotOpenFileStream,
    kFileOrDirectoryAlreadyExists,
    kCouldNotCreateFile,
    kParentDirectoryDoesNotExist,
    kInvalidPath,
    kCouldNotGetCurrentPath,
    kCouldNotSetCurrentPath,
    kCouldNotCreateHardLink,
    kCouldNotOpenDirectory,
    kCouldNotGetRealPath,
    kCouldNotCreateSymlink,
    kCouldNotChangeGroup,
    kCouldNotValidateGroup,
    kCouldNotGenerateTmpName,
    kCouldNotSyncDirectory,
    kCouldNotCompareFiles,
    kCorruptedFileSystem,
    kCouldNotFormatPartition,
    kCouldNotReadSymlink,
    kFsyncFailed,
    kCouldNotRenameFile,
    kNotImplemented,
};

score::result::Error MakeError(const ErrorCode code, const std::string_view user_message = "") noexcept;

}  // namespace filesystem
}  // namespace score

#endif  // SCORE_LIB_FILESYSTEM_ERROR_H
