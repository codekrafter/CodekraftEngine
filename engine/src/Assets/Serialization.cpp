#include "Serialization.hpp"

namespace ck
{

//template <class T>
//void saveProp(TrackedData data, T prop) {};

template<>
void saveProp<std::string>(TrackedData &data, std::string prop)
{
    LOG(INFO) << "Saving String";
    SizeS sz(prop.size() + 1);
    data.addData(&sz, sizeof(SizeS));
    data.addData(prop.c_str(), prop.size() +1);
}

template<>
void saveProp<signed int>(TrackedData &data, signed int prop)
{
    //SIntS si(prop);
    data.addData(&prop, sizeof(signed int));
}

// Basic sizeof for everything else, should expand exceptions as times get added
/*template<class T>
size_t getSize(T var)
{
    return sizeof(T);
}*/

size_t getSize(ck::Asset *var)
{
    return ck::AssetManager::inst()->getObject(var)->size();
}

size_t getSize(std::string var)
{
    return var.size() + 1 + sizeof(SizeS);
}

/*template<class V>
size_t getSize(std::vector<V> var)
{
    return sizeof(size_t) + var.size()*sizeof(V);
}*/

/*ShaderS::ShaderS(){};

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

};*/
} // namespace ck