#pragma once

#include <string>

const std::string v = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = vec4(aNormal,1.0f);
}
)";
/*const std::string f = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}

)";*/
// Solid color fragment shader
const std::string f = R"(
#version 330 core
out vec4 FragColor;

in vec4 vertexColor;

void main()
{    
    FragColor = vertexColor;
}

)";
