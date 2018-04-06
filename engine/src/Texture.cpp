#include <sstream>
#include <iomanip>
#include <string>

#include "Texture.hpp"
#include "ThirdParty/glad/glad.h"
#include "ThirdParty/stb_image.h"
#include "ThirdParty/easylogging/easylogging++.h"
#include "ThirdParty/stb_image_write.h"
#include "ThirdParty/cereal/types/vector.hpp"

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

// Create texture with specified number of components, 0 for max number in file.
Texture::Texture(std::string fname, int nn)
{
    version = 1;
    type = "TEXTURE";
    data = stbi_load(fname.c_str(), &width, &height, &n, 0);
    if (!data)
    {
        LOG(ERROR) << "failed to load image '" << fname << "'" << std::endl;
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

<<<<<<< HEAD
=======
/*template <class Archive>
void Texture::serialize(Archive &ar)
{
    LOG(INFO) << "serializing base class";
    ar(cereal::base_class<ck::Asset>(this));
    LOG(INFO) << "serialized base class";
    ar(width, height, n);
    LOG(INFO) << "(width,height,n)";
    LOG(INFO) << width;
    LOG(INFO) << height;
    LOG(INFO) << n;
    LOG(INFO) << "last element: " << data[0];
    ar(cereal::binary_data(data, width * height * n));
    LOG(INFO) << "finished serializing texture";
};*/
>>>>>>> 0cff946d6a15aadde52d8a42fbca0e1d803b90ba
template <class Archive>
void Texture::serialize(Archive &ar)
{
    if (!data)
    {
        LOG(ERROR) << "Tried to serialize image with no data";
    }
    std::cout << typeid(ar).name() << std::endl;
    //ar(cereal::base_class<ck::Asset>(this));
    ar(width, height, n);
    std::cout << width << std::endl;
    std::cout << height << std::endl;
    std::cout << n << std::endl;
    std::cout << width * height * n << std::endl;
    //ar(cereal::binary_data(data, width * height * n));
};
/*template <class Archive>
void Texture::save(Archive &ar) const
{
    LOG(ERROR) << "Texture saving is broken, doing nothing";
    //return;
    std::ostringstream ss;
    for (int i = 0; i < (width * height * n); ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << data[i] << ";";
    }
    std::string s = ss.str();
    //std::cout << s << std::endl;
    ar(s);
    ///std::vector<unsigned char> vec;
    //vec.resize(width*height*n);
    for (int i = 0; i < (width * height * n); ++i)
    {
        vec.push_back(data[i]);
    }
    //ar(vec);
    //ar(cereal::make_size_tag(static_cast<cereal::size_type>(width * height * n))); // number of elements
    //ar(cereal::binary_data(data, width * height * n));
    ar(width, height, n);
    //std::vector<unsigned char> vec(data, data + (width * height * n));
    //ar(vec);
    ar(cereal::make_size_tag(static_cast<cereal::size_type>(width * height * n))); // number of elements
    ar(cereal::binary_data(data, width * height * n));
}

template <class Archive>
void Texture::load(Archive &ar)
{
    LOG(ERROR) << "Texture loading is broken, doing nothing";
    //return;
    ar(width, height, n);

<<<<<<< HEAD
    ///std::vector<unsigned char> vec;
    //vec.resize(width * height * n);
    //ar(vec);
    //std::vector<unsigned char> vec(); //data, data + (width * height * n));
    //ar(vec);
    //cereal::size_type dSize;
    //ar(cereal::make_size_tag(dSize));

    //ar(cereal::binary_data(data, dSize));
}*/
=======
    //std::vector<unsigned char> vec(); //data, data + (width * height * n));
    //ar(vec);
    cereal::size_type dSize;
    ar(cereal::make_size_tag(dSize));

    ar(cereal::binary_data(data, width * height * n));
}
>>>>>>> 0cff946d6a15aadde52d8a42fbca0e1d803b90ba

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