/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <iostream>
#include <fstream>
#include "info.h"
#include "../info.h"
#include "exceptions.h"
#include "basicgeometry.h"

namespace cmd {

void Info::setOptions(cxxopts::Options &opts) {
    opts
    .positional_help("[args]")
    .custom_help("list *.JPG");
    
    opts.parse_positional({"input"});
}

std::string Info::description() {
    return "List files and directories";
}

void Info::run(cxxopts::ParseResult &opts) {
    if (!opts.count("input")) {
        printHelp();
    }

    auto input = opts["input"].as<std::vector<std::string>>();

    try{
        /*
        bool withHash = opts["with-hash"].count() > 0;
        auto format = opts["format"].as<std::string>();
        auto recursive = opts["recursive"].count() > 0;
        auto maxRecursionDepth = opts["depth"].as<int>();
        auto geometry = opts["geometry"].as<std::string>();

        if (opts.count("output")){
            std::string filename = opts["output"].as<std::string>();
            std::ofstream file(filename, std::ios::out | std::ios::trunc | std::ios::binary);
            if (!file.is_open()) throw ddb::FSException("Cannot open " + filename);

            ddb::info(input, file, format, recursive, maxRecursionDepth,
                      geometry, withHash, true);

            file.close();
        }else{
            ddb::info(input, std::cout, format, recursive, maxRecursionDepth,
                      geometry, withHash, true);
        }
        */
    }catch(ddb::InvalidArgsException){
        printHelp();
    }
}

}


