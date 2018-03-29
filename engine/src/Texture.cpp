#include "Texture.hpp"
#include "TP/glad/glad.h"
#include "TP/stb_image.h"
#include "TP/easylogging/easylogging++.h"
#include "TP/stb_image_write.h"

namespace ck
{
Texture::Texture()
{
    version = 1;
    type = "TEXTURE";
    LOG(DEBUG) << "making texure from thin air";
};

/*Texture::Texture(std::string fname)
{
    this->Texture(fname, 0);
};*/

// Create texture with specified number of components, 0 for max number in file.
Texture::Texture(std::string fname, int nn)
{
    version = 1;
    type = "TEXTURE";
    data = stbi_load(fname.c_str(), &width, &height, &n, nn);
    if (!data)
    {
        std::cerr << "failed to load image '" << fname << "'" << std::endl;
    }
    if (nn != 0)
    {
        n = nn;
    }
};
Texture::~Texture()
{
    stbi_image_free(data);
};

//template <class Archive>
//void Texture::serialize(Archive &ar)
//{
//    ar(/*cereal::base_class<ck::Asset>(this),*/ cereal::binary_data(data, (sizeof(data) * width)), width, height, n);
//};
template <class Archive>
void Texture::save(Archive &ar) const
{
    ar(width, height, n);
}

template <class Archive>
void Texture::load(Archive &ar)
{
    ar(width, height, n);
}
void Texture::init()
{
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
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, ID);
}
}