#include "Actor.hpp"

#include <cctype>

namespace ck
{
bool isDigit(char c)
{
    if (c == '1' ||
        c == '2' ||
        c == '3' ||
        c == '4' ||
        c == '5' ||
        c == '6' ||
        c == '7' ||
        c == '8' ||
        c == '9' ||
        c == '0')
    {
        return true;
    }
    else
    {
        return false;
    }
}

int toInt(char c)
{
    switch (c)
    {
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case '0':
        return 0;
    }

    return -1;
}

void Actor::onBeginPlay()
{
    for (ActorComponent *ac : components)
    {
        ac->setOwner(this);
    };
};

std::string Actor::getName()
{
    return name;
};

void Actor::setName(std::string n)
{
    std::vector<std::string> v = WorldManager::getInstance()->nameReg;
    if (std::find(v.begin(), v.end(), n) == v.end())
    {
        if (!(name == "" || name.empty() || name.size() == 0))
        {
            WorldManager::getInstance()->nameReg.erase(std::remove(WorldManager::getInstance()->nameReg.begin(),
                                                                   WorldManager::getInstance()->nameReg.end(), name),
                                                       WorldManager::getInstance()->nameReg.end());
        }

        name = n;
        WorldManager::getInstance()->nameReg.push_back(name);
    }
    else
    {
        char c = n[n.size() - 1];
        if (isDigit(c))
        {
            int num = toInt(c);
            num = num + 1;
            setName(n.substr(0, n.size() - 1) + std::to_string(num));
        }
        else
        {
            setName(n + "1");
        }
    }
};
} // namespace ck