#include "FileSelector.cpp"
#include "Parser.cpp"
#include "CodeGenerator.cpp"

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

    if (file.size() == 0)
    {
        std::cout << "No file specified" << std::endl;
        return -1;
    }

    //Generate file list
    ckb::FileSelection files = ckb::open(file);
    //Parse Files
    ckb::ParseOutput out = ckb::parseFiles(files);
    //TODO Generate code from serialization objects
    generateCode(out);
    return 0;
}
