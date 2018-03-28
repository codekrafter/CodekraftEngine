#include "Texture.hpp"
#include "TP/glad/glad.h"
#include "TP/stb_image.h"

namespace ck
{
Texture::Texture()
{
    version = 1;
    type = "TEXTURE";
};
Texture::Texture(std::string fname)
{
    version = 1;
    type = "TEXTURE";
    data = stbi_load(fname.c_str(), &width, &height, &n, 0);
    if (!data)
    {
        std::cerr << "failed to load image '" << fname << "'" << std::endl;
    }
};
Texture::~Texture()
{
    stbi_image_free(data);
};

template <class Archive>
void Texture::serialize(Archive &ar)
{
    ar(/*cereal::base_class<ck::Asset>(this),*/ *data, width, height, n);
};
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