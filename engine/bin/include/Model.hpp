#ifndef MODEL_H
#define MODEL_H

#include <TP/glad/glad.h>

#include <TP/glm/glm.hpp>
#include <TP/glm/gtc/matrix_transform.hpp>
#include <TP/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Mesh.hpp>
#include <Shader.hpp>
#include <TP/cereal/types/vector.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
namespace ck
{
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model : public Asset
{
public:
  /*  Model Data */
  vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  vector<Mesh *>* meshes;
  string directory;
  bool gammaCorrection;

  template <class Archive>
  void serialize(Archive &ar);

  /*  Functions   */
  // constructor, expects a filepath to a 3D model.
  Model(string const &path, bool gamma = false);
  Model();
  ~Model();

  // draws the model, and thus all its meshes
  void Draw(Shader *shader);

private:
  /*  Functions   */
  // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
  void loadModel(string const &path);

  // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
  void processNode(aiNode *node, const aiScene *scene);

  Mesh *processMesh(aiMesh *mesh, const aiScene *scene);

  // checks all material textures of a given type and loads the textures if they're not loaded yet.
  // the required info is returned as a Texture struct.
  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
}

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <TP/cereal/archives/binary.hpp>

CEREAL_REGISTER_TYPE(ck::Model);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::base_class, we need to clarify
//  the relationship (more on this later)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Model)

#endif