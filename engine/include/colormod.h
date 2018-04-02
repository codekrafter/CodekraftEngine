#pragma once

#include <ostream>

namespace ck
{
namespace term
{
namespace color
{
enum Code
{
    FG_RED = 91,//31, using light red for better visibility, actualy red is 31
    FG_GREEN = 32,
    FG_BLUE = 34,
    FG_DEFAULT = 39,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_BLUE = 44,
    BG_DEFAULT = 49
};

class Modifier
{
  private:
    Code code;

  public:
    Modifier(Code pCode) : code(pCode){};
    friend std::ostream &
    operator<<(std::ostream &os, const Modifier &mod)
    {
        return os << "\033[" << mod.code << "m";
    }
};
}
}
}