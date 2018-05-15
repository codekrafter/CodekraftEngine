#pragma once

#include <string>
#include <vector>
#include <map>

#include "Assets/Asset.hpp"

namespace ck
{

void empty_delete(Asset *);

struct AssetS;

template <class A>
class AssetRef;

class AssetManager
{
  private:
    template <class T>
    friend class AssetRef;
    std::map<std::string, std::map<std::string, Asset *>> map;
    std::vector<AssetRef<Asset>> refs;
    std::string prefix = "dat";
    std::string directory = "data";

    friend class CKEngine;
    AssetManager();
    static std::string processID(std::string id);
    uint32_t checksum(std::istream &stream);
    void flush();

    void savef(std::string name);
    void savef01(std::string name);

    void loadf(std::string name);
    void loadf01(std::vector<unsigned char> d, std::string cname);

    size_t getSize(std::string name);
    AssetS *getObject(Asset *a);
    AssetS *getObject(unsigned char UUID);

  public:
    ~AssetManager();
    static AssetManager *inst();
    void save();
    //void save(std::string dir);
    void load();
    //void load(std::string dir);
};

template <class A>
class AssetRef
{
  private:
    std::string ID;
    std::string chunk;

    void *operator new(size_t) { static_assert(true, "Do not create an AssetReference on the heap"); };           // standard new
    void *operator new(size_t, void *) { static_assert(true, "Do not create an AssetReference on the heap"); };   // placement new
    void *operator new[](size_t) { static_assert(true, "Do not create an AssetReference on the heap"); };         // array new
    void *operator new[](size_t, void *) { static_assert(true, "Do not create an AssetReference on the heap"); }; // placement array new

  public:
    AssetRef() : AssetRef("")
    {
        //LOG(WARNING) << "Creating blank asset reference, this is bad practice.";
        return;
    };

    AssetRef(std::string i, std::string c = "primary") : ID(i), chunk(c)
    {
        static_assert(std::is_base_of<Asset, A>::value, "A must be an asset");
    };

    /*AssetRef operator=(std::string i)
    {
        return AssetRef(i);
    };*/

    AssetRef
    operator=(std::pair<std::string, std::string> p)
    {
        ID = p.first;
        chunk = p.second;
    };

    bool operator==(const AssetRef &o)
    {
        //assert((ID == o.ID) && !(std::is_same<A,o.A>::value)); // IDs are the same, but they are of different types. This should never happen
        return (ID == o.ID && chunk == o.chunk);
    };

    A *operator->()
    {
        if (AssetManager::inst()->map[chunk].count(ID) == 0) // Do we need to make a new object?
        {
            AssetManager::inst()->map[chunk][ID] = new A();
        }

        return dynamic_cast<A *>(AssetManager::inst()->map[chunk][ID]);
        //return get();
    };

    A *get()
    {
        if (AssetManager::inst()->map[chunk].count(ID) == 0) // Do we need to make a new object?
        {
            AssetManager::inst()->map[chunk][ID] = new A();
        }

        return dynamic_cast<A *>(AssetManager::inst()->map[chunk][ID]);
    }
};
} // namespace ck