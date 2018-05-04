#include "Serialization.hpp"

namespace ck
{

ShaderS::ShaderS(){};

ShaderS::ShaderS(Shader *s)
{
    v = s->vertex;
    f = s->fragment;
    g = s->geometry;
};

Shader *ShaderS::asset()
{
    Shader *s = new Shader();
    s->vertex = v;
    s->fragment = f;
    s->geometry = g;
    return s;
};

DatSize ShaderS::save()
{
    size_t size = sizeof(SizeS) * 3 + v.size() + f.size() + g.size() + 3; // Size of the TriSize struct, sizes of all the strings plus 1 per string (store null termination)
    START_SAVE()

    //TRISIZE(v.size() + 1, f.size() + 1, g.size() + 1)

    S_SIZE(v.size() + 1)

    S_SIZE(f.size() + 1)

    S_SIZE(g.size() + 1)

    S_STRING(v)

    S_STRING(f)

    S_STRING(g)

    END_SAVE()
};

void ShaderS::load(unsigned char *data, size_t size)
{
    unsigned char *ptr = data;

    SizeS s1;
    SizeS s2;
    SizeS s3;

    std::memcpy(&s1, ptr, sizeof(SizeS));
    ptr = ptr + sizeof(SizeS);

    std::memcpy(&s2, ptr, sizeof(SizeS));
    ptr = ptr + sizeof(SizeS);

    std::memcpy(&s3, ptr, sizeof(SizeS));
    ptr = ptr + sizeof(SizeS);

    char *vv = (char *)malloc(s1.s);
    char *ff = (char *)malloc(s2.s);
    char *gg = (char *)malloc(s3.s);

    std::memcpy(vv, ptr, s1.s);
    ptr = ptr + s1.s;
    std::memcpy(ff, ptr, s2.s);
    ptr = ptr + s2.s;
    std::memcpy(gg, ptr, s3.s);
    ptr = ptr + s3.s;

    v = std::string(vv);
    f = std::string(ff);
    g = std::string(gg);

    free(vv);
    free(ff);
    free(gg);
};
}