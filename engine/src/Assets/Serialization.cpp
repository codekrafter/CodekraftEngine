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

    S_STRING(v)

    S_STRING(f)

    S_STRING(g)

    END_SAVE()
};

void ShaderS::load(unsigned char *data, size_t size){
    START_LOAD()

        L_STRING(v)

            L_STRING(f)

                L_STRING(g)

                    END_LOAD()

};
}