/* Copyright 2019 MasseranoLabs LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */
#ifndef INDEX_H
#define INDEX_H

#include <filesystem>
#include "../classes/database.h"
#include "../classes/statement.h"
#include "types.h"
#include "../classes/exif.h"
#include "../classes/hash.h"
#include "../classes/exceptions.h"
#include "../utils.h"
#include "entry.h"

namespace fs = std::filesystem;

namespace ddb {

std::string create(const std::string &directory);
std::unique_ptr<Database> open(const std::string &directory, bool traverseUp);
std::vector<fs::path> getPathList(fs::path rootDirectory, const std::vector<std::string> &paths, bool includeDirs);
bool checkUpdate(Entry &e, const fs::path &p, long long dbMtime, const std::string &dbHash);
void doUpdate(Statement *updateQ, const Entry &e);

void addToIndex(Database *db, const std::vector<std::string> &paths);
void removeFromIndex(Database *db, const std::vector<std::string> &paths);
void syncIndex(Database *db);


}


#endif // INDEX_H