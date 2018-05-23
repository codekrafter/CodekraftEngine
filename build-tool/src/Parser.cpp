#pragma once

#include <map>

#include "FileSelector.cpp"
#include "CKObject.hpp"

namespace ckb
{
struct ParseOutput
{
    std::map<std::string, std::vector<CKObject>> files;
};
std::vector<CKObject> parseFile(std::string file);

ParseOutput parseFiles(FileSelection fs)
{
    std::vector<std::string> files = fs.files;
    std::map<std::string, std::vector<CKObject>> files_map;
    ParseOutput out;

    for (std::string &f : files)
    {
        std::vector<CKObject> objs = parseFile(f);
        if (!objs.empty())
        {
            files_map[f] = objs;
        }
    }

    out.files = files_map;

    return out;
}

std::string checkLine(std::string line);
std::string getClassName(std::string line);
std::pair<std::string, std::string> getTypeName(std::string line);

std::vector<CKObject> parseFile(std::string file)
{
    std::ifstream is(file);
    if (!is)
    {
        return std::vector<CKObject>();
    }
    std::vector<CKObject> objs;
    {
        std::string line;
        int ln = 1;

        // Current State
        CKObject currentOBJ;
        unsigned short nextItem = 0;

        while (std::getline(is, line))
        {
            std::string macro = checkLine(line);
            if (nextItem > 0)
            {
                if (nextItem == 1)
                {
                    auto lfc = line.find("class");
                    if (line.find("class") == std::string::npos)
                    {
                        std::cout << "LINE: " << line << std::endl;
                        std::cout << "Found CKClass Macro, but did not find class declaration in next line" << std::endl;
                        std::cout << "File: " << file << " Line: " << ln << std::endl;
                    }
                    std::string name = getClassName(line);
                    currentOBJ.name = name;
                    nextItem = 0;
                }
                else if (nextItem == 2)
                {
                    std::pair<std::string, std::string> var = getTypeName(line);

                    currentOBJ.vars[var.first] = var.second;
                    nextItem = 0;
                }
                else if (nextItem == 3)
                {
                    std::pair<std::string, std::string> var = getTypeName(line);

                    currentOBJ.events[var.first] = var.first;
                    nextItem = 0;
                }
                else
                {
                    std::cout << "Unknown Macro ID '" << nextItem << "'" << std::endl;
                    nextItem = 0;
                }
            }
            else if (macro.size() > 0)
            {
                //std::cout << "found CK Macro in file '" << file << "' at line " << ln << std::endl;
                // Cleansed Macro
                std::string cmacro = macro.substr(0, macro.find("("));
                //std::cout << cmacro << std::endl;
                if (cmacro == "CKClass")
                {
                    CKObject dummy;
                    if (currentOBJ != dummy)
                    {
                        objs.push_back(currentOBJ);
                        currentOBJ = CKObject();
                    }
                    nextItem = 1;
                } /*
                else if (cmacro == "CKStruct")
                {
                }*/
                else if (cmacro == "CKProperty")
                {
                    nextItem = 2;
                }
                else if (cmacro == "CKEvent")
                {
                    nextItem = 3;
                }
                else
                {
                    std::cout << "Unknown Property: '" << cmacro << "'" << std::endl;
                }
            }
            ln = ln + 1;
        }
        {
            CKObject dummy;
            if (currentOBJ != dummy)
            {
                objs.push_back(currentOBJ);
                currentOBJ = CKObject();
            }
        }
    }
    return objs;
}

std::string getClassName(std::string line)
{
    std::string p1 = line.substr(line.find("class") + 5, line.size() - 5); // - 5 /*line.find("class")*/);
    std::string p2;
    for (char c : p1)
    {
        if (std::isspace(c))
        {
        }
        else
        {
            if (c == ':')
            {
                break;
            }
            else
            {
                p2 = p2 + c;
            }
        }
    }
    return p2;
}

std::pair<std::string, std::string> getTypeName(std::string line)
{
    int spaces = 0;
    int normal = 0;
    std::string out;
    std::string type;
    out.clear();
    type.clear();
    for (char &c : line)
    {
        if (std::isspace(c))
        {
            if (!(normal < 1))
            {
                spaces++;
            }

            if (spaces > 1 && !out.empty())
            {
                return std::make_pair(out, type);
            }
        }
        else
        {
            normal++;
            if (spaces >= 1)
            {
                //std::cout << c << std::endl;
                out = out + c;
            }
            else
            {
                type = type + c;
            }
        }
    }
    return std::make_pair(out, type);
}

std::string checkLine(std::string line)
{
    if (std::isspace(line[0]))
    {
        return checkLine(line.substr(1, line.size() + 1));
    }
    else
    {
        bool is_macro = false;
        is_macro = (line.substr(0, 2) == "CK");
        is_macro = is_macro && (line.find("(") != std::string::npos);
        if (is_macro == false)
            return "";
        is_macro = is_macro && (line.substr(0, line.find("(")).find(":") == std::string::npos);
        is_macro = is_macro && (line.substr(0, 8) != "CKEngine");
        return is_macro ? line : "";
    }
}

} // namespace ckb