#pragma once

#include <string>
#include <vector>
#include <map>

#include "Asset.hpp"

namespace ck
{
struct AssetS;

template <class A>
class AssetRef;

class AssetManager
{
  private:
    friend class AssetRef<Asset>;
    std::map<std::string, std::map<std::string, Asset *>> map;
    std::vector<AssetRef<Asset>> refs;
    std::string prefix;
    std::string suffix = "dat";
    std::string directory = "data";

    AssetManager();
    static AssetManager *iinst;
    static std::string processID(std::string id);
    uint32_t checksum(std::istream &stream);
    void flush();

    void savef();
    void loadf();

    size_t getSize(std::string name);
    AssetS *getObject(std::string name);

  public:
    ~AssetManager();
    static AssetManager *inst();
    void cleanup();
    void unload();
    void unload(std::string name);
    void load();
    void load(std::string name);
};

template <class A>
class AssetRef
{
  private:
    std::string ID;
    std::string chunk;

  public:
    AssetRef()
    {
        static_assert(std::is_base_of<Asset, A>::value, "A must be an asset");
    };

    AssetRef(std::string i, std::string c = "primary") : ID(i), chunk(c){};

    AssetRef operator=(std::string i)
    {
        return AssetRef(i);
    };

    AssetRef
    operator=(std::pair<std::string, std::string> p)
    {
        ID = p.first;
        chunk = p.second;
    };

    bool operator==(const AssetRef &o)
    {
        //assert((ID == o.ID) && !(std::is_same<A,o.A>::value)); // IDs are the same, but they are of different types. This should never happen
        return (ID == o.ID);
    };

    Asset *operator->()
    {
        if (AssetManager::inst()->map[chunk].count(ID) == 0) // Do we need to make a new object?
        {
            //if (AssetManager::inst()->checkFile(chunk)) // Chunk file exists but is not loaded?
            //AssetManager::inst()->map[chunk][ID] = new A();
        }
        return AssetManager::inst()->map[chunk][ID];
    };
};
}