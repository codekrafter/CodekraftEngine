#pragma once

#include <utility>
#include <stdlib.h>

#include "StaticMesh.hpp"

namespace ck
{

struct DatSize
{
    unsigned char *data;
    size_t size;
};

struct AssetS
{
    // Export binary data, and size
    virtual DatSize save() = 0;
    virtual void load(unsigned char *data, size_t size) = 0;
    // IMPORTANT: Each AssetS should also define a constructor that takes the corresponding asset, and a "deconstructor" that returns a corresponding asset.
};

struct TriSize
{
    size_t n1;
    size_t n2;
    size_t n3;
};

struct ShaderS : AssetS
{
    ShaderS(){};
    ShaderS(Shader *s)
    {
        v = s->vertex;
        f = s->fragment;
        g = s->geometry;
    };
    virtual Shader *asset()
    {
        Shader *s = new Shader();
        s->vertex = v;
        s->fragment = f;
        s->geometry = g;
        return s;
    };

    virtual DatSize save()
    {
        unsigned char *out;
        size_t size = sizeof(TriSize) + v.size() + f.size() + g.size() + 3; // Size of the TriSize struct, sizes of all the strings plus 1 per string (store null termination)
        out = (unsigned char *)malloc(size);
        TriSize ts;
        ts.n1 = v.size() + 1;
        ts.n2 = f.size() + 1;
        ts.n3 = g.size() + 1;
        unsigned char *ptr = out;
        std::memcpy(ptr, &ts, sizeof(TriSize));
        ptr = ptr + sizeof(TriSize);

        std::memcpy(ptr, v.c_str(), ts.n1);
        LOG(DEBUG) << ptr;
        ptr = ptr + ts.n1;
        std::memcpy(ptr, f.c_str(), ts.n2);
        LOG(DEBUG) << ptr;
        ptr = ptr + ts.n2;
        std::memcpy(ptr, g.c_str(), ts.n3);
        LOG(DEBUG) << ptr;
        ptr = ptr + ts.n3;
        DatSize o;
        o.data = out;
        o.size = size;
        return o;
    };
    virtual void load(unsigned char *data, size_t size)
    {
        TriSize ts;
        unsigned char *ptr = data;
        std::memcpy(&ts, ptr, sizeof(TriSize));
        ptr = ptr + sizeof(TriSize);

        char *vv = (char *)malloc(ts.n1);
        char *ff = (char *)malloc(ts.n2);
        char *gg = (char *)malloc(ts.n3);

        std::memcpy(vv, ptr, ts.n1);
        ptr = ptr + ts.n1;
        std::memcpy(ff, ptr, ts.n2);
        ptr = ptr + ts.n2;
        std::memcpy(gg, ptr, ts.n3);
        ptr = ptr + ts.n3;

        v = std::string(vv);
        f = std::string(ff);
        g = std::string(gg);

        free(vv);
        free(ff);
        free(gg);
    };

    std::string v;
    std::string f;
    std::string g;
};

struct TriInt
{
    int width, height, n;
};

struct TextureS : AssetS
{
    TextureS(){};
    TextureS(Texture *tex)
    {
        ti.width = tex->width;
        ti.height = tex->height;
        ti.n = tex->n;
        data = tex->data;
    };
    virtual Texture *asset()
    {
        Texture *tex = new Texture();
        tex->width = ti.width;
        tex->height = ti.height;
        tex->n = ti.n;
        tex->data = data;
        return tex;
    };

    TriInt ti;
    unsigned char *data;

    virtual DatSize save()
    {
        size_t size = sizeof(TriInt) + (ti.width * ti.height * ti.n);
        unsigned char *out = (unsigned char *)malloc(size);
        unsigned char *ptr = out;

        std::memcpy(ptr, &ti, sizeof(TriInt));
        ptr = ptr + sizeof(TriInt);

        std::memcpy(ptr, data, (ti.width * ti.height * ti.n));
        ptr = ptr + (ti.width * ti.height * ti.n);

        DatSize o;
        o.size = size;
        o.data = out;
        return o;
    };
    virtual void load(unsigned char *in, size_t size)
    {
        unsigned char *ptr = in;

        std::memcpy(&ti, ptr, sizeof(TriInt));
        ptr = ptr + sizeof(TriInt);

        if (data != nullptr)
        {
            free(data);
        }

        data = (unsigned char *)malloc(ti.width * ti.height * ti.n);

        std::memcpy(data, ptr, (ti.width * ti.height * ti.n));
        ptr = ptr + (ti.width * ti.height * ti.n);
    };
};

void empty_delete(Asset *){};

struct MaterialS : AssetS
{
    ShaderS shader;
    TextureS diffuse;
    TextureS specular;

    MaterialS(){};
    MaterialS(Material *mat)
    {
        shader = mat->shader.get();
        diffuse = mat->diffuse.get();
        specular = mat->specular.get();
    };
    virtual Material *asset()
    {
        Material *mat = new Material();
        mat->shader = std::shared_ptr<Shader>(shader.asset(), empty_delete);
        mat->diffuse = std::shared_ptr<Texture>(diffuse.asset(), empty_delete);
        mat->specular = std::shared_ptr<Texture>(specular.asset(), empty_delete);
        return mat;
    };

    virtual DatSize save()
    {
        DatSize ShaderDat = shader.save();
        DatSize DiffuseDat = diffuse.save();
        DatSize SpecularDat = specular.save();
        size_t size = sizeof(TriSize) + ShaderDat.size + DiffuseDat.size + SpecularDat.size;
        unsigned char *data = (unsigned char *)malloc(size);
        unsigned char *ptr = data;

        TriSize ts;
        ts.n1 = ShaderDat.size;
        ts.n2 = DiffuseDat.size;
        ts.n3 = SpecularDat.size;
        std::memcpy(ptr, &ts, sizeof(TriSize));
        ptr = ptr + sizeof(TriSize);

        std::memcpy(ptr, ShaderDat.data, ts.n1);
        ptr = ptr + ts.n1;
        free(ShaderDat.data);

        std::memcpy(ptr, DiffuseDat.data, ts.n2);
        ptr = ptr + ts.n2;
        free(DiffuseDat.data);

        std::memcpy(ptr, SpecularDat.data, ts.n3);
        ptr = ptr + ts.n3;
        free(SpecularDat.data);

        DatSize o;
        o.size = size;
        o.data = data;
        return o;
    };
    virtual void load(unsigned char *data, size_t size)
    {
        unsigned char *ptr = data;

        DatSize ShaderDat;
        DatSize DiffuseDat;
        DatSize SpecularDat;

        TriSize ts;
        std::memcpy(&ts, ptr, sizeof(TriSize));
        ptr = ptr + sizeof(TriSize);

        ShaderDat.data = (unsigned char *)malloc(ts.n1);
        std::memcpy(ShaderDat.data, ptr, ts.n1);
        ptr = ptr + ts.n1;
        shader.load(ShaderDat.data, ShaderDat.size);
        free(ShaderDat.data);

        DiffuseDat.data = (unsigned char *)malloc(ts.n2);
        std::memcpy(DiffuseDat.data, ptr, ts.n2);
        ptr = ptr + ts.n2;
        diffuse.load(DiffuseDat.data, DiffuseDat.size);
        free(DiffuseDat.data);

        SpecularDat.data = (unsigned char *)malloc(ts.n3);
        std::memcpy(SpecularDat.data, ptr, ts.n3);
        ptr = ptr + ts.n3;
        specular.load(SpecularDat.data, SpecularDat.size);
        free(SpecularDat.data);
    };
};

struct Vec3S
{
    float x;
    float y;
    float z;
    Vec3S(){};
    Vec3S(glm::vec3 glm)
    {
        x = glm.x;
        y = glm.y;
        z = glm.z;
    }
    glm::vec3 glm()
    {
        return glm::vec3(x, y, z);
    }
};

struct Vec2S
{
    float x;
    float y;
    Vec2S(){};
    Vec2S(glm::vec2 glm)
    {
        x = glm.x;
        y = glm.y;
    }
    glm::vec2 glm()
    {
        return glm::vec2(x, y);
    }
};

struct VertexS
{
    Vec3S pos;
    Vec3S norm;
    Vec2S uv;
    Vec3S tan;
    Vec3S bitan;
    VertexS(Vertex v)
    {
        pos = v.Position;
        norm = v.Normal;
        uv = v.TexCoords;
        tan = v.Tangent;
        bitan = v.Bitangent;
    }
    virtual Vertex asset()
    {
        Vertex v;
        v.Position = pos.glm();
        v.Normal = norm.glm();
        v.TexCoords = uv.glm();
        v.Tangent = tan.glm();
        v.Bitangent = bitan.glm();
        return v;
    };
};

struct MeshS : AssetS
{
    std::vector<VertexS> vertices;
    std::vector<unsigned int> indices;
    MaterialS mat;
    inline bool operator<(const MeshS &rhs) const { return (vertices.size() < rhs.vertices.size()); };
    MeshS(){};
    MeshS(Mesh *mesh)
    {
        //std::copy(mesh->vertices.begin(), mesh->vertices.end(), std::back_inserter(vertices));
        for (Vertex v : mesh->vertices)
        {
            vertices.push_back(VertexS(v));
        }
        indices = mesh->indices;
        mat = mesh->mat;
    };
    virtual Mesh *asset()
    {
        Mesh *m = new Mesh();
        for (VertexS vs : vertices)
        {
            m->vertices.push_back(vs.asset());
        }
        m->indices = indices;
        m->mat = mat.asset();
        return m;
    }

    virtual DatSize save()
    {
        DatSize matDat = mat.save();
        size_t size = sizeof(TriSize) + vertices.size() + indices.size() + matDat.size;
        unsigned char *out;
        out = (unsigned char *)malloc(size);
        unsigned char *ptr = out;
        TriSize ts;
        ts.n1 = matDat.size;
        ts.n2 = vertices.size();
        ts.n3 = indices.size();
        std::memcpy(ptr, &ts, sizeof(TriSize));
        ptr = ptr + sizeof(TriSize);

        std::memcpy(ptr, matDat.data, ts.n1);
        ptr = ptr + ts.n1;
        free(matDat.data);

        std::memcpy(ptr, &vertices[0], ts.n2 * sizeof(VertexS));
        ptr = ptr + ts.n2;
        std::memcpy(ptr, &indices[0], ts.n3 * sizeof(unsigned int));
        ptr = ptr + ts.n2;

        DatSize o;
        o.data = out;
        o.size = size;
        return o;
    };
    virtual void load(unsigned char *data, size_t size)
    {
        unsigned char *ptr = data;
        TriSize ts;
        std::memcpy(&ts, ptr, sizeof(TriSize));
        ptr = ptr + sizeof(TriSize);

        DatSize matDat;
        matDat.data = (unsigned char *)malloc(ts.n1);
        matDat.size = ts.n1;
        std::memcpy(matDat.data, ptr, ts.n1);
        ptr = ptr + ts.n1;
        mat.load(matDat.data, matDat.size);
        free(matDat.data);

        vertices.reserve(ts.n2);
        std::memcpy(&vertices[0], ptr, ts.n2 * sizeof(VertexS));
        ptr = ptr + ts.n2;

        vertices.reserve(ts.n3);
        std::memcpy(&indices[0], ptr, ts.n3 * sizeof(unsigned int));
        ptr = ptr + ts.n3;
    };
};

struct SizeS
{
    size_t s;

    SizeS(size_t ss) : s(ss){};
    SizeS() : s(0){};
};

struct StaticMeshS : AssetS
{
    StaticMeshS(){};
    StaticMeshS(StaticMesh *sm)
    {
        //std::copy(sm->meshes.begin(), sm->meshes.end(), std::back_inserter(meshes));
        for (Mesh *m : sm->meshes)
        {
            meshes.push_back(MeshS(m));
        }
    };
    virtual StaticMesh *asset()
    {
        StaticMesh *sm = new StaticMesh();
        //std::copy(meshes.begin(), meshes.end(), std::back_inserter(sm->meshes));
        for (MeshS ms : meshes)
        {
            sm->meshes.push_back(ms.asset());
        }
        return sm;
    }
    std::vector<MeshS> meshes;

    virtual DatSize save()
    {
        std::map<MeshS, DatSize> ds;
        for (MeshS m : meshes)
        {
            ds[m] = m.save();
        }
        size_t size = sizeof(SizeS);
        for (std::pair<MeshS, DatSize> d : ds)
        {
            size = size + sizeof(SizeS) + d.second.size;
        }

        unsigned char *data = (unsigned char *)malloc(size);
        unsigned char *ptr = data;

        {
            SizeS s = meshes.size();
            std::memcpy(ptr, &s, sizeof(SizeS));
            ptr = ptr + sizeof(SizeS);
        }

        for (std::pair<MeshS, DatSize> d : ds)
        {
            SizeS s = d.second.size;
            std::memcpy(ptr, &s, sizeof(SizeS));
            ptr = ptr + sizeof(SizeS);
            std::memcpy(ptr, d.second.data, s.s);
            ptr = ptr + s.s;
        }

        DatSize o;
        o.size = size;
        o.data = data;
        return o;
    };
    virtual void load(unsigned char *data, size_t size)
    {
        unsigned char *ptr = data;
        SizeS msize = meshes.size();

        std::memcpy(&size, ptr, sizeof(SizeS));
        ptr = ptr + sizeof(SizeS);

        for (int i = 0; i < msize.s; i++)
        {
            SizeS s;
            std::memcpy(&s, ptr, sizeof(SizeS));
            ptr = ptr + sizeof(SizeS);

            unsigned char *d = (unsigned char *)malloc(s.s);

            std::memcpy(d, ptr, s.s);
            ptr = ptr + s.s;

            MeshS mesh;
            mesh.load(d, s.s);
            meshes.push_back(mesh);

            free(d);
        }
    };
};
}