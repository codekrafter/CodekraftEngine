#pragma once

#include "GLM.hpp"

namespace ck
{
struct RenderingConfig;

struct DirVectors
{
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp = glm::vec3(0.0f, 0.0f, 1.0f);
};

DirVectors getVectors(float yaw, float pitch, glm::vec3 worldUp = glm::vec3(0.0f, 0.0f, 0.1f));

glm::mat4 getViewMatrix(glm::vec3 pos, DirVectors dv);

glm::mat4 getProjection(RenderingConfig rc, float zoom = 70.0f);

} // namespace ck