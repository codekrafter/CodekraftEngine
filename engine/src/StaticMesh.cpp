#include "StaticMesh.hpp"

namespace ck
{

StaticMesh::StaticMesh()
{
    version = 1;
    type = "STATICMESH";
};
StaticMesh::StaticMesh(std::string path)
{
    version = 1;
    type = "STATICMESH";
    // Initialize Loader
    objl::Loader Loader;

    // Load .obj File
    bool loaded = Loader.LoadFile(path);

    // Check to see if it loaded

    if (!loaded)
    {
        throw std::runtime_error("Could not load .obj file");
        return;
    };

    // Go through each loaded mesh and add it to render pool
    for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
    {
        meshes.push_back(new ck::Mesh(Loader.LoadedMeshes[i]));
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
    for (Mesh *mesh : meshes)
    {
        delete mesh;
        mesh = nullptr;
    }
};
template <class Archive>
void StaticMesh::serialize(Archive &ar)
{
    std::vector<std::shared_ptr<Mesh>> ms;
    for (Mesh *mesh : meshes)
    {
        ms.push_back(std::shared_ptr<Mesh>(mesh));
    }
    ar(cereal::base_class<ck::Asset>(this), ms);
    meshes.clear();
    for (std::shared_ptr<Mesh> m : ms)
    {
        meshes.push_back(m.get());
    }
};
std::vector<Mesh *> StaticMesh::getMeshes()
{
    return meshes;
}

void StaticMesh::init()
{
    for (Mesh *mesh : meshes)
    {
        mesh->init();
    }
};
void StaticMesh::draw()
{
    for (Mesh *mesh : meshes)
    {
        mesh->draw();
    }
};
}