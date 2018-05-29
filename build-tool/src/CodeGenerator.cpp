#pragma once

#include <sys/types.h> // required for stat.h
#include <sys/stat.h>  // no clue why required -- man pages say so
#include <iomanip>

void createDirectory(std::string sPath)
{
    mode_t nMode = 0733; // UNIX style permissions
    int nError = 0;
#if defined(_WIN32)
    nError = _mkdir(sPath.c_str()); // can be used on Windows
#else
    nError = mkdir(sPath.c_str(), nMode); // can be used on non-Windows
#endif
    if (nError != 0)
    {
        // handle your error here
    }
}

#include "Parser.cpp"
#include "Template.cpp.in"

std::vector<std::string> split(const std::string &str, const std::string &delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos)
            pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty())
            tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

namespace ckb
{

std::string generateLookup(std::vector<CKObject> objs)
{
    std::stringstream file;

    file << "#include <vector>" << std::endl
         << std::endl
         << std::endl;

    for (CKObject o : objs)
    {
        file << "#include \"" << o.name << ".generated.hpp\"" << std::endl;
    }
    file << std::endl;

    file << "namespace ck" << std::endl
         << "{" << std::endl
         << "struct AssetS;" << std::endl
         << "}" << std::endl

         << "namespace ckg" << std::endl
         << "{" << std::endl;
    for (CKObject o : objs)
    {
        file << "\t"
             << "struct " << o.name << "S;" << std::endl;
    }
    file << "}" << std::endl
         << std::endl
         << std::endl
         << std::endl;

    file << "namespace ck" << std::endl
         << "{" << std::endl;

    file << "std::vector<AssetS*> getGeneratedObjects()" << std::endl
         << "{" << std::endl
         << "\t"
         << "std::vector<AssetS*> out;" << std::endl
         << std::endl;

    file << "\t"
         << "return out;" << std::endl
         << "}" << std::endl;

    file << "}" << std::endl;

    return file.str();
}

std::pair<std::string, std::string> generateCode(CKObject obj, std::string fileName, unsigned char UUID);
void generateCode(ParseOutput parsed)
{
    createDirectory("generated");
    unsigned char cUUID = 0x0001;
    std::vector<CKObject> totalObjects;
    for (std::pair<std::string, std::vector<CKObject>> p : parsed.files)
    {
        std::vector<std::string> path = split(p.first, "/");
        std::string fileName = "./generated/" + split(path[path.size() - 1], ".")[0];
        for (CKObject o : p.second)
        {
            totalObjects.push_back(o);
            std::cout << "Generating  " << o.name << std::endl;
            std::pair<std::string, std::string> code = generateCode(o, fileName, cUUID);
            std::ofstream os(fileName + ".generated.hpp", std::ios::trunc);
            os << code.first;
            os.close();
            os.open(fileName + ".generated.cpp", std::ios::trunc);
            os << code.second;
            os.close();
            cUUID = cUUID + 0x0001;
        }
    }

    std::ofstream os("./generated/Lookup.generated.cpp", std::ios::trunc);
    os << generateLookup(totalObjects);
    os.close();
    return;
};

std::string calculateSize(CKObject obj);

std::pair<std::string, std::string> generateCode(CKObject obj, std::string fileName, unsigned char UUID)
{
    std::stringstream hpp;
    std::stringstream cpp;

    // Heading
    hpp << "#ifndef CKOBJECT_HEADING" << std::endl
        << "#define CKOBJECT_HEADING" << std::endl
        << include_heading << std::endl
        << "#endif" << std::endl
        << std::endl;

    // Start Object
    hpp << "namespace ckg" << std::endl
        << "{" << std::endl
        << "struct " << obj.name << "S : AssetS" << std::endl
        << "{" << std::endl
        << "  private:" << std::endl;
    // Fields
    for (std::pair<std::string, std::string> var : obj.vars)
    {
        hpp << "\t" << var.second << " " << var.first << ";" << std::endl;
    }
    // Serialization Functions
    hpp << "\t" << obj.name << "S() {};" << std::endl
        << "\t" << obj.name << "S(" << obj.name << "* a)" << std::endl
        << "\t"
        << "virtual " << obj.name << "* asset();" << std::endl
        << "\t"
        << "virtual DatSize save() override;" << std::endl
        << "\t"
        << "virtual void load(unsigned char *data, size_t size) override;" << std::endl
        << "\t"
        << "virtual inline unsigned char getUUID() override;" << std::endl
        << "\t"
        << "virtual inline " << obj.name << "S *clone() override;" << std::endl;
    // End Object
    hpp << "};" << std::endl
        << "}" << std::endl;

    // Start Source
    cpp << "#include \"" << fileName << ".generated.hpp"
        << "\"" << std::endl
        << std::endl
        << std::endl
        << std::endl
        << "namespace ckg " << std::endl
        << "{" << std::endl;
    // Asset Constructor
    cpp << obj.name << "S::" << obj.name << "S(" << obj.name << "* a)" << std::endl
        << "{" << std::endl;
    for (std::pair<std::string, std::string> var : obj.vars)
    {
        cpp << "\t" << var.first << " = a->" << var.first << ";" << std::endl;
    }
    cpp << "}" << std::endl
        << std::endl;
    // Asset Creator
    cpp << "virtual " << obj.name << "* " << obj.name << "S::asset()" << std::endl
        << "{" << std::endl
        << "\t" << obj.name << " *a = new" << obj.name << "();" << std::endl
        << std::endl;
    for (std::pair<std::string, std::string> var : obj.vars)
    {
        cpp << "\t"
            << "a->" << var.first << " = " << var.first << ";" << std::endl;
    }
    cpp << std::endl;
    cpp << "\t"
        << "return a;" << std::endl
        << "}" << std::endl
        << std::endl;
    // Save
    cpp << "DatSize " << obj.name << "S::save()" << std::endl
        << "{" << std::endl
        << "\t"
        << "size_t size = " << calculateSize(obj) << std::endl
        << "\t"
        << "START_SAVE()" << std::endl;

    for (std::pair<std::string, std::string> p : obj.vars)
    {
        cpp << "\t";
        if (p.second == "std::string")
        {
            cpp << "S_STRING(" << p.first << ")";
        }
        else
        {
            std::cout << "WARNING: Trying to save property of type '" << p.second << "' Which is not known by the build tool. Using simple S_STRUCT, not the best for complex objects" << std::endl;
            cpp << "S_STRUCT(" << p.first << ")";
        };
        cpp << std::endl;
    }

    cpp << "\t"
        << "END_SAVE()" << std::endl
        << "}" << std::endl;

    cpp << std::endl;

    // Load
    cpp << "void " << obj.name << "S::load(unsigned char *data, size_t size)" << std::endl
        << "{" << std::endl
        << "\t"
        << "START_LOAD()" << std::endl;

    for (std::pair<std::string, std::string> p : obj.vars)
    {
        cpp << "\t";
        if (p.second == "std::string")
        {
            cpp << "L_STRING(" << p.first << ")";
        }
        else
        {
            std::cout << "WARNING: Trying to load property of type '" << p.second << "' Which is not known by the build tool. Using simple L_STRUCT, not the best for complex objects" << std::endl;
            cpp << "L_STRUCT(" << p.first << ")";
        };
        cpp << std::endl;
    }

    cpp << "\t"
        << "END_LOAD()" << std::endl
        << "}" << std::endl;

    cpp << std::endl;

    // Get UUID
    cpp << "inline unsigned char" << obj.name << "S::getUUID()" << std::endl
        << "{" << std::endl
        << "\t"
        << "return 0x" << std::hex << std::setfill('0') << std::setw(4) << static_cast<int>(UUID) << ";" << std::endl
        << "}" << std::endl;

    cpp << std::endl;

    // Clone
    cpp << "inline " << obj.name << "S *" << obj.name << "S::clone()" << std::endl
        << "{" << std::endl
        << "\t"
        << "return new " << obj.name << "S();" << std::endl
        << "}" << std::endl;
    cpp << "}" << std::endl;

    // Return Source Files
    return std::make_pair(hpp.str(), cpp.str());
}

std::string calculateSize(CKObject obj)
{
    std::string out;
    for (std::pair<std::string, std::string> pair : obj.vars)
    {
        if (pair.second == "std::string")
        {
            out = out + "sizeof(size_t) + " + pair.first + ".size()";
        }
        else
        {
            std::cout << "No custom size for type: '" << pair.second << "'. Will use sizeof" << std::endl;
            out = out + "sizeof(" + pair.second + ")";
        }
        out = out + " + ";
    }
    out = out + "0;";
    return out;
}
} // namespace ckb