#include "CameraUtils.hpp"
#include "Core/Config.hpp"

namespace ck
{

DirVectors getVectors(float yaw, float pitch, glm::vec3 worldUp /* = glm::vec3(0.0f, 0.0f, 0.1f)*/)
{
    DirVectors dv;
    // Calculate the front vector
    dv.front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dv.front.y = sin(glm::radians(pitch));
    dv.front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    dv.front = glm::normalize(dv.front);
    // Also re-calculate the Right and Up vector
    dv.right = glm::normalize(glm::cross(dv.front, worldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    dv.up = glm::normalize(glm::cross(dv.right, dv.front));
    dv.worldUp = worldUp;
    return dv;
};

glm::mat4 getViewMatrix(glm::vec3 pos, DirVectors dv)
{
    return glm::lookAt(pos, pos + dv.front, dv.up);
};

glm::mat4 getProjection(RenderingConfig rc, float zoom /* = 70.0f*/)
{
    return glm::perspective(glm::radians(zoom), (float)rc.width / (float)rc.height, 0.1f, 100.0f);
};

} // namespace ck