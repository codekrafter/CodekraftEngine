#pragma once

#include <map>
#include <string>

namespace ckb
{

struct CKObject
{
    // Name : Type
    std::map<std::string, std::string> vars;

    // Label : callable name
    std::map<std::string, std::string> events;

    std::string name;

    void reset()
    {
        vars.clear();
        events.clear();
        name.clear();
    }
    bool operator==(const CKObject &o)
    {
        return (vars == o.vars) && (events == o.events) && (name == o.name);
    }
    bool operator!=(const CKObject &o)
    {
        return !operator==(o);
    }
};

} // namespace ckb