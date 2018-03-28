#pragma once

#include <TP/glm/glm.hpp>
#include <TP/cereal/types/memory.hpp>

namespace glm
{
template <class Archive>
void serialize(Archive &archive,
               vec2 &m)
{
    archive(m[0], m[1]);
}
template <class Archive>
void serialize(Archive &archive,
               vec3 &m)
{
    archive(m[0], m[1], m[2]);
}
}