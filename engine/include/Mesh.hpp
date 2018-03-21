#pragma once

#include "TP/glm/glm.hpp"
#include "Asset.hpp"
#include "TP/OBJ_Loader.h"
#include "TP/cereal/types/vector.hpp"

namespace glm
{
template <class Archive>
void serialize(Archive &archive,
               vec2 &m)
{
    archive(m[0], m[1]);
}
template <class Archive>
void serialize(Archive &archive,
               vec3 &m)
{
    archive(m[0], m[1], m[2]);
}
}
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
    Mesh();
    Mesh(objl::Mesh m);
    ~Mesh();
    template <class Archive>
    void serialize(Archive &archive);
    void init();
    void draw();
};
}

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <TP/cereal/archives/binary.hpp>

CEREAL_REGISTER_TYPE(ck::Mesh);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::base_class, we need to clarify
//  the relationship (more on this later)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Mesh)