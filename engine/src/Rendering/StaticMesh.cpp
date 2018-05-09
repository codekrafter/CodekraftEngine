#include "Rendering/StaticMesh.hpp"
#include "Assets/AssetManager.hpp"
#include "ThirdParty/cereal/types/vector.hpp"

namespace ck
{

StaticMesh::StaticMesh()
{
    version = 1;
    type = "STATIC_MESH";
    meshes = std::vector<Mesh *>();
};
StaticMesh::StaticMesh(std::string path)
{
    version = 1;
    type = "STATIC_MESH";
    loadRaw(path);
};

void StaticMesh::loadRaw(std::string path)
{
    // Initialize Loader
    objl::Loader Loader;

    // Load .obj File
    bool loaded = Loader.LoadFile(path);
    // Check to see if it loaded

    if (!loaded)
    {
        LOG(ERROR) << "Could not load '" << path << "' for a static mesh";
        return;
    };

    //empty_delete<Mesh> ed;
    // Go through each loaded mesh and add it to render pool
    for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
    {
        std::string dir = path.substr(0, path.find_last_of('/'));
        dir = dir + "/";
        meshes.push_back(new ck::Mesh(Loader.LoadedMeshes[i], dir));
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

StaticMesh::~StaticMesh(){
    /*for (Mesh *mesh : meshes)
    {
        delete mesh;
        mesh = nullptr;
    }*/
};

std::vector<Mesh *> StaticMesh::getMeshes()
{
    std::vector<Mesh *> ms;
    for (Mesh *m : meshes)
    {
        ms.push_back(m);
    }
    return ms;
}

void StaticMesh::init()
{
    for (Mesh *mesh : meshes)
    {
        mesh->init();
    }
};
void StaticMesh::draw(glm::mat4 model)
{
    for (Mesh *mesh : meshes)
    {
        mesh->draw(model);
    }
};
}