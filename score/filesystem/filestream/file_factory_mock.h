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
#ifndef BASELIBS_SCORE_FILESYSTEM_FILESTREAM_FILE_FACTORY_MOCK_H
#define BASELIBS_SCORE_FILESYSTEM_FILESTREAM_FILE_FACTORY_MOCK_H

#include "score/filesystem/error.h"
#include "score/filesystem/filestream/i_file_factory.h"
#include "score/filesystem/path.h"

#include <ios>
#include <istream>
#include <memory>

#include <gmock/gmock.h>

namespace score
{
namespace filesystem
{

class FileFactoryMock : public IFileFactory
{
  public:
    MOCK_METHOD(score::Result<std::unique_ptr<std::iostream>>,
                Open,
                (const Path&, std::ios_base::openmode),
                (noexcept, override));
};

}  // namespace filesystem
}  // namespace score

#endif  // BASELIBS_SCORE_FILESYSTEM_FILESTREAM_FILE_FACTORY_MOCK_H
