#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

namespace ckb
{

struct FileSelection
{
    std::vector<std::string> files;
};

FileSelection open(std::string path_to_database)
{
    FileSelection fs;
    std::ifstream is(path_to_database);
    json j;
    is >> j;

    for (auto &a : j)
    {
        std::string fname = a["file"];
        fs.files.push_back(fname);
        if (fname.find(".cpp") != std::string::npos)
        {
            std::string header = fname.substr(0, fname.find(".cpp")) + ".hpp";
            fs.files.push_back(header);
        }
    }
    return fs;
};

} // namespace ckb