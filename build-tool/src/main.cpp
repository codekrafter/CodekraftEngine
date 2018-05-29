#include <unistd.h>
#include <regex>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include "FileSelector.cpp"
#include "Parser.cpp"
#include "CodeGenerator.cpp"

inline bool file_exists(const std::string &name)
{
    if (FILE *file = fopen(name.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

#ifdef __WIN32__
#error Need to create install engine system for windows
bool checkEngineInstall()
{
    return false;
}
#else
bool checkEngineInstall()
{
    return file_exists("/opt/CodekraftEngine/scripts/build.js");
}
#endif

int main(int argc, char *argv[])
{
    //Parse Arguments
    std::vector<std::string> flags;
    std::string file;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg.substr(0, 2) == "--")
        {
            flags.push_back(arg.substr(2, arg.size() - 2));
        }
        else if (arg.substr(0, 1) == "-")
        {
            flags.push_back(arg.substr(1, arg.size() - 1));
        }
        else
        {
            if (file.size() > 0)
            {
                std::cout << "Duplicate file name '" << arg << "' after '" << file << "'" << std::endl;
                return -1;
            }
            else
            {
                file = arg;
            }
        }
    }
    if (file.size() < 1)
        file = ".";
    bool generateOnly = false;
    if (std::find(flags.begin(), flags.end(), "g") != flags.end())
    {
        generateOnly = true;
    }

    if (std::find(flags.begin(), flags.end(), "genonly") != flags.end())
    {
        generateOnly = true;
    }

    if (std::find(flags.begin(), flags.end(), "go") != flags.end())
    {
        generateOnly = true;
    }
    std::regex dir("^\\.{1,2}\\/*$");
    bool is_dir = false;
    if (file.substr(file.size() - 1, 1) == "/")
    {
        is_dir = true;
    }

    if (std::regex_match(file, dir))
    {
        is_dir = true;
        file = file + "/";
    }

    if (is_dir)
    {
        std::vector<std::string> files;
        for (auto &p : fs::recursive_directory_iterator(file))
        {
            if (p.path().extension() == ".ckproj")
                files.push_back(p.path());
        }

        if (files.size() > 1)
        {
            std::cout << "There is more than one project configuration file in the specified directory, going with the first one (" << files[0] << ")" << std::endl;
            file = files[0];
        }
        else
        {
            file = files[0];
        }
    }
    if (generateOnly)
    {
        //Generate file list
        ckb::FileSelection files = ckb::open(file);
        //Parse Files
        ckb::ParseOutput out = ckb::parseFiles(files);
        //TODO Generate code from serialization objects
        generateCode(out);
    }
    else
    {
        std::string flags_str;
        for (std::string f : flags)
        {
            flags_str = flags_str + "-" + f + " ";
        }
        if (checkEngineInstall())
        {
#ifdef __WIN32__
#error Need to implement calling engine script
#else
            system(("/opt/CodekraftEngine/scripts/build.js " + std::string(file) + " " + flags_str).c_str());
#endif
        }
        else
        {
            std::cout << "Could not find engine, please check your engine installation" << std::endl;
        }
    }
    return 0;
}
