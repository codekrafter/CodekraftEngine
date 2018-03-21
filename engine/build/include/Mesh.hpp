#ifndef MESH_H
#define MESH_H

#include <TP/glad/glad.h> // holds all OpenGL type declarations

#include <TP/glm/glm.hpp>
#include <TP/glm/gtc/matrix_transform.hpp>

#include <Shader.hpp>

#include <TP/cereal/types/vector.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

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
    Vertex(){};
    Vertex(const Vertex &other)
    {
        Position = other.Position;
        Normal = other.Normal;
        TexCoords = other.TexCoords;
        Tangent = other.Tangent;
        Bitangent = other.Bitangent;
    }
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(Position[0], Position[1], Position[2], Normal[0], Normal[1], Normal[0], TexCoords[0], TexCoords[1], Tangent[0], Tangent[1], Tangent[2], Bitangent[0], Bitangent[1], Bitangent[2]);
    };
};

struct Texture
{
    unsigned int id;
    string type;
    string path;
    Texture(){};
    Texture(const Texture &other)
    {
        id = other.id;
        type = other.type;
        path = other.path;
    };
    template <class Archive>
    void serialize(Archive &ar)
    {
        ar(type, path);
    };
};

class Mesh
{
  public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;

    template <class Archive>
    void serialize(Archive &ar)
    {
        ar(vertices, indices, textures);
    };

    /*  Functions  */
    // constructor
    Mesh(){};
    Mesh(const Mesh &other)
    {
        //std::cout << "!!Calling copy constructor" << std::endl;
        vertices = other.vertices;
        indices = other.indices;
        textures = other.textures;
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
    };
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(Shader *shader)
    {
        std::cout << "drawing from mesh" << std::endl;
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        std::cout << "# of textures: " << textures.size() << std::endl;
        if (textures.size() > 0)
        {
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                std::cout << "started loop i: " << i << std::endl;
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                std::cout << "activated" << std::endl;
                string number;
                string name = textures[i].type;
                std::cout << "retrived name" << std::endl;
                if (name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++); // transfer unsigned int to stream
                else if (name == "texture_normal")
                    number = std::to_string(normalNr++); // transfer unsigned int to stream
                else if (name == "texture_height")
                    number = std::to_string(heightNr++); // transfer unsigned int to stream
                std::cout << "ifelsedone" << std::endl;
                // now set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }

  private:
    /*  Render data  */
    unsigned int VBO, EBO;

    /*  Functions    */
};
}
#endif