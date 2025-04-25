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
#ifndef BASELIBS_SCORE_FILESYSTEM_FILESYSTEM_H
#define BASELIBS_SCORE_FILESYSTEM_FILESYSTEM_H

// Contains headers for filesystem library users (except headers for fakes and mocks).

// IStandardFilesystem
#include "score/filesystem/details/standard_filesystem.h"
#include "score/filesystem/i_standard_filesystem.h"
// IFileUtils
#include "score/filesystem/file_utils/file_utils.h"
#include "score/filesystem/file_utils/i_file_utils.h"
// IFileFactory
#include "score/filesystem/filestream/file_factory.h"
#include "score/filesystem/filestream/i_file_factory.h"
// FilesystemFactory
#include "score/filesystem/factory/filesystem_factory.h"
// directory iterators
#include "score/filesystem/iterator/directory_iterator.h"
#include "score/filesystem/iterator/recursive_directory_iterator.h"

#endif  // BASELIBS_SCORE_FILESYSTEM_FILESYSTEM_H
