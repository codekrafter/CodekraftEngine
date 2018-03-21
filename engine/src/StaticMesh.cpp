#include "StaticMesh.hpp"

namespace ck
{

StaticMesh::StaticMesh(){};
StaticMesh::StaticMesh(std::string path)
{
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

int StaticMesh::getVersion()
{
    return 1;
}

std::string StaticMesh::getType()
{
    return "STATICMESH";
}

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
    ar(ms);
};
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