#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "AssetManager.hpp"
#include "ThirdParty/tinydir.h"
#include "AssetStructures.hpp"
#include "ThirdParty/CRC.h"

#include "Logging.hpp"

namespace ck
{

void empty_delete(Asset *){};

#define VERIFY(condition, message) \
    {                              \
        if (!condition)            \
        {                          \
            LOG(ERROR) << message; \
            return;                \
        }                          \
    }

#define VERIFY_FATAL(condition, message) \
    {                                    \
        if (!condition)                  \
        {                                \
            LOG(FATAL) << message;       \
            return;                      \
        }                                \
    }

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd // stupid MSFT "deprecation" warning
#elif __unix__
#include <unistd.h>
#endif

#ifdef _WIN32
// creates the directory, returns if the directory was created
bool createDirectory(const char *path)
{
#error Someones needs to implement a create directory function for windows here!
}
#elif __unix__
#include <unistd.h>
struct stat st = {0};
// creates the directory, returns if the directory was created
bool createDirectory(const char *path)
{
    if (stat(path, &st) == -1)
    {
        mkdir(path, 0700);
        return true;
    }
    else
    {
        return false;
    }
}
#endif

unsigned char getUUID(Asset *a)
{
    if (typeid(*a) == typeid(Shader))
    {
        return 0x0001;
    }
    else if (typeid(*a) == typeid(Mesh))
    {
        return 0x0002;
    }
    else if (typeid(*a) == typeid(Material))
    {
        return 0x0003;
    }
    else if (typeid(*a) == typeid(Texture))
    {
        return 0x0004;
    }
    else if (typeid(*a) == typeid(StaticMesh))
    {
        return 0x0005;
    }
    else
    {
        LOG(ERROR) << "Could not find UUID of asset type: '" << typeid(a).name() << "'";
        return 0x0000;
    }
};

AssetS *AssetManager::getObject(unsigned char UUID)
{

    if (UUID == 0x0001)
    {
        return new ShaderS();
    }
    else if (UUID == 0x0002)
    {
        return new MeshS();
    }
    else if (UUID == 0x0003)
    {
        return new MaterialS();
    }
    else if (UUID == 0x0004)
    {
        return new TextureS();
    }
    else if (UUID == 0x0005)
    {
        return new StaticMeshS();
    }
    else
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(4) << static_cast<int>(UUID);
        LOG(ERROR) << "Could not find asset of UUID: 0x" << ss.str();
        return nullptr;
    }
};

AssetManager *AssetManager::iinst;

AssetManager::AssetManager(){};

AssetManager::~AssetManager(){};

// Takes a string as an asset ID, and makes sure there is no whitespace, and it is all lowercase
std::string AssetManager::processID(std::string i)
{
    std::string ID = i;
    for (char c : ID) // Check to make sure there are no whitespace characters in the ID, this could cause problems down the road
    {
        assert(!isspace(c));
    }
    std::transform(ID.begin(), ID.end(), ID.begin(), ::tolower); // Make ID lowercase, again to prevent problems down the road
    return ID;
};

AssetManager *AssetManager::inst()
{
    if (iinst == nullptr)
    {
        iinst = new AssetManager();
    }
    return iinst;
};

void AssetManager::cleanup()
{
    delete iinst;
    iinst = nullptr;
};

void AssetManager::flush()
{
    return;
};

void AssetManager::save()
{
    for (std::pair<std::string, std::map<std::string, Asset *>> p : map)
    {
        savef(p.first);
    }
};

/*void AssetManager::save(std::string name)
{
    flush();
};*/

void AssetManager::load()
{
    tinydir_dir dir;
    tinydir_open(&dir, ("./" + directory).c_str());
    std::vector<tinydir_file> files;
    while (dir.has_next)
    {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        std::string name = file.name;
        if (
            name.substr(0, prefix.size()) == prefix &&
            name.find(".ckd") != std::string::npos &&
            name.substr(name.find(".ckd"), std::string::npos) == ".ckd")
        {
            files.push_back(file);
        }

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
    if (files.size() < 1)
    {
        LOG(INFO) << "no data files in data directory";
    }
    for (tinydir_file f : files)
    {
        loadf(f.name);
    }
};

/*void AssetManager::load(std::string name)
{
    flush();
};*/

void AssetManager::savef(std::string name)
{
    // Always save with latest version
    savef01(name);
}

void AssetManager::savef01(std::string name)
{
    std::map<std::string, Asset *> chunk = map[name];
    SizeS s_chunk = chunk.size();

    std::vector<unsigned char> header;

    // Start of header
    header.push_back(0x0001);

    // Unique ID Code
    header.push_back(0x0043);
    header.push_back(0x004B);
    header.push_back(0x0044);

    // End of transmisson block
    header.push_back(0x0017);

    // Version
    header.push_back(0x0001);

    // End of transmission block
    header.push_back(0x0017);

    // Size of Chunk
    {
        size_t s = header.size();
        /*for (int i = 0; i < sizeof(SizeS); ++i)
        {
            header.push_back(0x0000);
        }*/
        header.resize(s + sizeof(SizeS));

        assert(header.size() - s == sizeof(SizeS));
        std::memcpy(&header[s], &s_chunk, sizeof(SizeS));
    }

    std::map<Asset *, DatSize> dsv;
    std::map<Asset *, std::string> IDLookup;

    for (std::pair<std::string, Asset *> pair : chunk)
    {

        AssetS *as = getObject(pair.second);
        DatSize ds = as->save();
        dsv[pair.second] = ds;
        IDLookup[pair.second] = pair.first;
        delete as;
    };

    // CRC Lookup table
    CRC::Table<std::uint32_t, 32> table(CRC::CRC_32());

    for (std::pair<Asset *, DatSize> p : dsv)
    {
        // UUID for Asset Type
        header.push_back(getUUID(p.first));

        // Size of Asset
        {
            size_t s = header.size();
            SizeS s_asset = p.second.size;
            header.resize(s + sizeof(SizeS));
            /*for (int i = 0; i < sizeof(SizeS); ++i)
            {
                header.push_back(0x0000);
            }*/
            assert(header.size() - s == sizeof(SizeS));
            std::memcpy(&header[s], &s_asset, sizeof(SizeS));
            LOG(INFO) << "old size: " << s_asset.s;
        }

        // 32-bit CRC of asset saved data
        {
            std::uint32_t crc;
            //unsigned char bytes[4];
            crc = CRC::Calculate(p.second.data, p.second.size, table);
            LOG(INFO) << "old crc:" << crc;
            for (int i = 0; i != 4; ++i)
            {
                unsigned char b = (crc >> (24 - i * 8)) & 0xFF;
                //bytes[i] = b;
                header.push_back(b);
            }
        }

        // ID
        {
            std::string ID = IDLookup[p.first];
            const char *cstring = ID.c_str();
            // Size
            SizeS size = strlen(cstring);
            size_t s = header.size();
            header.resize(s + sizeof(SizeS));
            assert(header.size() - s == sizeof(SizeS));
            std::memcpy(&header[s], &size, sizeof(SizeS));

            // Data (cstring)
            for (int i = 0; i < size.s; ++i)
            {
                header.push_back(cstring[i]);
            }
        }
    };

    // End of Transmission Block
    header.push_back(0x0017);

    size_t pre_size = header.size();
    size_t expected_size = 0;
    size_t es2 = 0;
    for (std::pair<Asset *, DatSize> p : dsv)
    {
        expected_size = expected_size + p.second.size;
        // Group Seperator
        //header.push_back(0x001D);

        for (int i = 0; i < p.second.size; ++i)
        {
            header.push_back(p.second.data[i]);
            es2 = es2 + 1;
        }
    };
    size_t post_size = header.size();
    size_t total_size = post_size - pre_size;
    assert(total_size == expected_size && expected_size == es2);

    // End of Transmission
    header.push_back(0x0004);

    // Save data
    std::string fname = "./" + directory + "/" + prefix + name + ".ckd";
    // Create the directory if it does not already exist
    createDirectory(directory.c_str());
    std::ofstream of(fname, std::ios::out | std::ios::binary | std::ios::trunc);
    if (of.is_open())
    {
        of.write((char *)&header[0], header.size() * sizeof(unsigned char));
    }
    else
    {
        LOG(ERROR) << "Failed to open asset file for saving";
        LOG(ERROR) << "Error: " << strerror(errno);
    }
    of.close();

    return;
};

void AssetManager::loadf(std::string name)
{
    std::string cname = name.substr(prefix.size(), name.size() - 7);
    LOG(INFO) << "cname: " << cname;

    std::string fname = "./" + directory + "/" + name;
    std::ifstream is(fname, std::ios::in | std::ios::binary | std::ios::ate);
    if (is.is_open())
    {
        std::streamsize size = is.tellg();
        is.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (is.read(buffer.data(), size))
        {
            std::vector<unsigned char> data(buffer.begin(), buffer.end());

            // Check Header
            VERIFY((data[0] == 0x0001 &&
                    data[1] == 0x0043 &&
                    data[2] == 0x004B &&
                    data[3] == 0x0044 &&
                    data[4] == 0x0017),
                   "File does not start with correct heading")

            unsigned char version = data[5];
            VERIFY((data[6] == 0x0017), "File does not have correct spacing after version")

            switch (version)
            {
            case 0x0001:
                loadf01(data, cname);
                break;

            default:
                std::stringstream ss;
                ss << std::hex << std::setfill('0') << std::setw(4) << static_cast<int>(data[5]);
                LOG(ERROR) << "Unknown version: 0x" << ss.str();
                return;
            }
        }
    }
    else
    {
        LOG(ERROR) << "Failed to open asset file for loading";
        LOG(ERROR) << "Error: " << strerror(errno);
        return;
    }
    /*
    // Start of header
    header.push_back(0x0001);

    // Unique ID Code
    header.push_back(0x0043);
    header.push_back(0x004B);
    header.push_back(0x0044);

    // End of transmisson block
    header.push_back(0x0017);

 */ return;
};

struct HeaderEntry
{
    unsigned char UUID;
    std::uint32_t hash;
    size_t size;
    std::string ID;
};

void AssetManager::loadf01(std::vector<unsigned char> d, std::string cname)
{
    unsigned char *data = (unsigned char *)malloc(d.size());
    std::copy(d.begin(), d.end(), data);
    unsigned char *ptr = data;

    ptr = ptr + 7;

    // Load number of header entries (So number of assets in chunk)
    SizeS s_chunk;
    std::memcpy(&s_chunk, ptr, sizeof(SizeS));
    ptr = ptr + sizeof(SizeS);

    // CRC Table
    CRC::Table<std::uint32_t, 32> table(CRC::CRC_32());

    // Load Header
    std::vector<HeaderEntry> header;
    std::map<std::string, Asset *> chunk;
    for (int i = 0; i < s_chunk.s; ++i)
    {
        unsigned char UUID = *ptr;
        ptr = ptr + 1;

        LOG(INFO) << "UUID: 0x" << std::hex << std::setfill('0') << std::setw(4) << static_cast<short>(UUID);

        // Size
        SizeS as;
        std::memcpy(&as, ptr, sizeof(SizeS));
        ptr = ptr + sizeof(SizeS);
        LOG(INFO) << "new size: " << std::dec << as.s;

        // CRC/Hash/Checksum
        unsigned char *bytes = ptr;
        std::uint32_t crc = 0;
        for (int ii = 0; ii < 4; ++ii)
        {
            crc |= (std::uint32_t)bytes[ii] << (24 - ii * 8);
        }
        ptr = ptr + 4;
        LOG(INFO) << "new crc:" << crc;

        // ID
        SizeS id_size;
        std::memcpy(&id_size, ptr, sizeof(SizeS));
        ptr = ptr + sizeof(SizeS);
        std::vector<char> buffer;
        for (int ii = 0; ii < id_size.s; ++ii)
        {
            buffer.push_back(*ptr);
            ptr = ptr + 1;
        }

        std::string ID(buffer.begin(), buffer.end());
        LOG(INFO) << "ID: " << ID;

        HeaderEntry he;
        he.UUID = UUID;
        he.hash = crc;
        he.size = as.s;
        he.ID = ID;
        header.push_back(he);
    }

    // End of Transmission Block
    ++ptr;

    for (HeaderEntry h : header)
    {
        DatSize ds;
        ds.size = h.size;
        ds.data = (unsigned char *)malloc(ds.size);

        std::memcpy(ds.data, ptr, ds.size);
        ptr = ptr + ds.size;

        //Check CRC
        std::uint32_t crc = CRC::Calculate(ds.data, ds.size, table);
        if (!(crc == h.hash))
        {
            LOG(ERROR) << "Error while loading asset: checksums do not match";
        }

        AssetS *as = getObject(h.UUID);
        as->load(ds.data, ds.size);

        chunk[h.ID] = as->asset();

        delete as;

        free(ds.data);
    }

    free(data);
    data = nullptr;
    ptr = nullptr;
    map[cname] = chunk;
};

size_t AssetManager::getSize(std::string name)
{
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);

    if (name == "SHADER")
    {
        return sizeof(ShaderS);
    }
    else if (name == "MESH")
    {
        return sizeof(MeshS);
    }
    else if (name == "MATERIAL")
    {
        return sizeof(MaterialS);
    }
    else if (name == "TEXTURE")
    {
        return sizeof(TextureS);
    }
    else if (name == "STATIC_MESH")
    {
        return sizeof(StaticMeshS);
    }
    else
    {
        LOG(ERROR) << "Could not find asset for name: '" << name << "'";
        return 0;
    }
};

AssetS *AssetManager::getObject(Asset *a)
{

    if (typeid(*a) == typeid(Shader))
    {
        Shader *s = reinterpret_cast<Shader *>(a);
        return new ShaderS(s);
    }
    else if (typeid(*a) == typeid(Mesh))
    {
        Mesh *m = reinterpret_cast<Mesh *>(a);
        return new MeshS(m);
    }
    else if (typeid(*a) == typeid(Material))
    {
        Material *m = reinterpret_cast<Material *>(a);
        return new MaterialS(m);
    }
    else if (typeid(*a) == typeid(Texture))
    {
        Texture *t = reinterpret_cast<Texture *>(a);
        return new TextureS(t);
    }
    else if (typeid(*a) == typeid(StaticMesh))
    {
        StaticMesh *s = reinterpret_cast<StaticMesh *>(a);
        return new StaticMeshS(s);
    }
    else
    {
        LOG(ERROR) << "Could not find asset of type: '" << typeid(a).name() << "'";
        return nullptr;
    }
};
}