#include "StaticMesh.hpp"
#include "AssetManager.hpp"
#include "TP/easylogging/easylogging++.h"
#include "TP/cereal/types/vector.hpp"

namespace ck
{

StaticMesh::StaticMesh()
{
    version = 1;
    type = "STATICMESH";
    LOG(DEBUG) << "making static mesh from thin air";
};
StaticMesh::StaticMesh(std::string path)
{
    LOG(DEBUG) << "making static mesh";
    version = 1;
    type = "STATICMESH";
    // Initialize Loader
    objl::Loader Loader;

    // Load .obj File
    LOG(DEBUG) << "loading file...";
    bool loaded = Loader.LoadFile(path);
    std::cout << "loaded" << std::endl;
    // Check to see if it loaded

    if (!loaded)
    {
        throw std::runtime_error("Could not load .obj file");
        return;
    };

    empty_delete<Mesh> ed;
    // Go through each loaded mesh and add it to render pool
    for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
    {
        std::string dir = path.substr(0, path.find_last_of('/'));
        dir = dir + "/";
        meshes.push_back(std::shared_ptr<Mesh>(new ck::Mesh(Loader.LoadedMeshes[i], dir), ed));
    }
};

std::string StaticMesh::getType()
{
    return type;
}
int StaticMesh::getVersion()
{
    return version;
};

StaticMesh::~StaticMesh()
{
    for (std::shared_ptr<Mesh> mesh : meshes)
    {
        delete mesh.get();
        mesh = nullptr;
    }
};
template <class Archive>
void StaticMesh::serialize(Archive &ar)
{
    ar(cereal::base_class<ck::Asset>(this), meshes);
};
std::vector<Mesh *> StaticMesh::getMeshes()
{
    std::vector<Mesh *> ms;
    for (std::shared_ptr<Mesh> m : meshes)
    {
        ms.push_back(m.get());
    }
    return ms;
}

void StaticMesh::init()
{
    for (std::shared_ptr<Mesh> mesh : meshes)
    {
        mesh->init();
    }
};
void StaticMesh::draw(Transform trans)
{
    for (std::shared_ptr<Mesh> mesh : meshes)
    {
        mesh->draw(trans);
    }
};
}