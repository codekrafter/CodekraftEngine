#pragma once

#include "ThirdParty/glm/glm.hpp"
#include "Asset.hpp"
#include "ThirdParty/OBJ_Loader.h"
#include "ThirdParty/cereal/types/vector.hpp"
#include "Material.hpp"
#include "GLMSavers.hpp"

namespace ck
{
struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(Position, Normal, TexCoords, Tangent);
    }
};

class Mesh : public Asset
{
  private:
    unsigned int VBO, VAO, EBO;
    //glm::mat4 trans;
    std::vector<ck::Vertex> vertices;
    std::vector<unsigned int> indices;

  public:
    Material *mat;
    Mesh();
    Mesh(objl::Mesh m, std::string directory);
    virtual ~Mesh();
    template <class Archive>
    void serialize(Archive &archive);
    virtual void init();
    void draw(Transform trans);
};
}

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <ThirdParty/cereal/types/polymorphic.hpp>
#include <ThirdParty/cereal/archives/portable_binary.hpp>

CEREAL_REGISTER_TYPE(ck::Mesh);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::virtual_base_class, we need to clarify
//  the relationship (more on this later)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Mesh)