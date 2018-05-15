#pragma once

#include <vector>

#include "GLM.hpp"

namespace ck
{
    struct Light
    {
        glm::vec3 pos;
        glm::vec3 color;
    };
    struct SceneInfo
    {
        std::vector<Light> lights;
    };
}