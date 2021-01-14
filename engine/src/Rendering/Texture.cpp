#include <sstream>
#include <iomanip>
#include <string>

#include "Texture.hpp"
#include "ThirdParty/glad/glad.h"
#include "ThirdParty/stb_image.h"
#include "ThirdParty/stb_image_write.h"
#include "ThirdParty/cereal/types/vector.hpp"

#include "Logging.hpp"

namespace ck
{
Texture::Texture()
{
    version = 1;
    type = "TEXTURE";
};

/*Texture::Texture(std::string fname)
{
    this->Texture(fname, 0);
};*/

Texture::Texture(std::string fname, int nn)
{
    version = 1;
    type = "TEXTURE";
    loadRaw(fname, nn);
};

// Create texture with specified number of components, 0 for max number in file.
void Texture::loadRaw(std::string fname, int nn)
{
    path = fname + "/" + std::to_string(nn);
    data = nullptr;
    width = 0;
    height = 0;
    n = 0;
    if (fname == "")
    {
        return;
    }
    data = stbi_load(fname.c_str(), &width, &height, &n, 0);
    if (!data || data == nullptr)
    {
        LOG(LOG_ERROR) << "failed to load image '" << fname << "'" << std::endl;
    }
    /*if (nn != 0)
    {
        n = nn;
    }*/
};
Texture::~Texture()
{
    stbi_image_free(data);
};

void Texture::init()
{
    //LOG(DEBUG) << "Running Texture::init() for texture of path '" << path << "'";
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
};

void Texture::draw(int i)
{
    //LOG(DEBUG) << "Running Texture::draw() for texture of path '" << path << "'";

    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, ID);
}
}