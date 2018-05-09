#include "Mesh.hpp"
#include "Material.hpp"
#include "GLM.hpp"
#include "Assets/Asset.hpp"
#include "ThirdParty/OBJ_Loader.h"
#include "ThirdParty/glad/glad.h"
#include "ECS/Transform.hpp"
#include "ThirdParty/cereal/types/vector.hpp"

namespace ck
{
Mesh::Mesh(){};
Mesh::Mesh(objl::Mesh m, std::string directory)
{
    for (int j = 0; j < m.Vertices.size(); j++)
    {
        ck::Vertex v;
        v.Position = glm::vec3(m.Vertices[j].Position.X, m.Vertices[j].Position.Y, m.Vertices[j].Position.Z);
        v.Normal = glm::vec3(m.Vertices[j].Normal.X, m.Vertices[j].Normal.Y, m.Vertices[j].Normal.Z);
        v.TexCoords = glm::vec2(m.Vertices[j].TextureCoordinate.X, m.Vertices[j].TextureCoordinate.Y);
        vertices.push_back(v);
    }
    objl::Material mmat = m.MeshMaterial;
    mat = new Material(mmat.map_Kd, mmat.map_Ks, directory);
    indices = m.Indices;
};
Mesh::~Mesh()
{
    delete mat;
};
/*template <class Archive>
void Mesh::serialize(Archive &archive)
{
    archive(cereal::base_class<ck::Asset>(this), vertices, indices, *mat);
};*/
void Mesh::init()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
    mat->init();
};
void Mesh::draw(glm::mat4 model)
{
    mat->draw(model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
};
}