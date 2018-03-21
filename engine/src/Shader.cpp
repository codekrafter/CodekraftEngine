#include <TP/glad/glad.h>
#include <TP/glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "Shader.hpp"
#include "AssetFactory.hpp"
#include "AssetManager.hpp"

namespace ck
{

Shader::Shader()
{
    version = 1;
    type = "SHADER";
    ///*
    *vertex = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";
    /*fragment = R"(
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
    fragment = new std::string(R"(
#version 330 core
out vec4 FragColor;

void main()
{    
    FragColor = vec4(0.6f,0.6f,0.6f,1.0f);
}

)");
    *geometry = "";
    //*/
}
Shader::~Shader()
{
    delete vertex;
    delete fragment;
    delete geometry;
};

/*void Shader::deserialize(std::string data)
{
    std::vector<std::string> lines = AssetManager::split_string(data, "\n");
    std::string sel = "";
    for (std::string line : lines)
    {
        if (line.substr(0, 2) == "!!")
        {
            sel = line.substr(2, line.length());
        }
        else
        {
            if (sel == "")
            {
                if (line != "")
                {
                    throw std::runtime_error("Shader Error: Code text '" + line + "'appeared before section selector");
                }
            }
            else if (sel == "VERTEX")
            {
                *vertex = *vertex + line + "\n";
            }
            else if (sel == "FRAGMENT")
            {
                *fragment = *fragment + line + "\n";
            }
            else if (sel == "GEOMETRY")
            {
                *geometry = *geometry + line + "\n";
            }
            else
            {
                throw std::runtime_error("Shader Error: Invalid Shader type: '" + sel + "'");
            }
        }
    };
}
std::string Shader::serialize()
{
    std::ostringstream s;
    s << "!!VERTEX" << std::endl
      << vertex << std::endl
      << "!!FRAGMENT" << std::endl
      << fragment << std::endl
      << "!!GEOMETRY" << std::endl
      << geometry << std::endl;
    return s.str();
}*/
template <class Archive>
void Shader::serialize(Archive &ar)
{
    ar(cereal::base_class<ck::Asset>(this), type, version, *vertex, *fragment, *geometry);
}

void Shader::init()
{
    // 2. compile shaders
    unsigned int vid, fid;
    int success;
    char infoLog[512];
    const char *vs = vertex->c_str();
    const char *fs = fragment->c_str();
    const char *gs = geometry->c_str();
    // vertex shader
    vid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vid, 1, &vs, NULL);
    glCompileShader(vid);
    checkCompileErrors(vid, "VERTEX");
    // fragment Shader
    fid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fid, 1, &fs, NULL);
    glCompileShader(fid);
    checkCompileErrors(fid, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    unsigned int gid;
    if (*geometry != "")
    {
        gid = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gid, 1, &gs, NULL);
        glCompileShader(gid);
        checkCompileErrors(gid, "GEOMETRY");
    }
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vid);
    glAttachShader(ID, fid);
    if (*geometry != "")
        glAttachShader(ID, gid);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vid);
    glDeleteShader(fid);
    if (*geometry != "")
        glDeleteShader(gid);
};

std::string Shader::getType()
{
    return type;
}
int Shader::getVersion()
{
    return version;
};
// activate the shader
// ------------------------------------------------------------------------
void Shader::use()
{
    glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
}