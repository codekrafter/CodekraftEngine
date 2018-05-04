#include "ThirdParty/glad/glad.h"
#include "GLM.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "ThirdParty/cereal/types/string.hpp"

#include "Rendering/Shader.hpp"
#include "Assets/Asset.hpp"
#include "DefaultShaderCode.hpp"
//#include "Assets/AssetManager.hpp"

namespace ck
{

Shader::Shader()
{
    version = 1;
    type = "SHADER";
    vertex = v;
    fragment = f;
    geometry = "";
}

void Shader::setCode(std::string v, std::string f, std::string g)
{
    if (v != "")
    {
        vertex = v;
    }
    if (f != "")
    {
        fragment = f;
    }
    if (g != "")
    {
        geometry = g;
    }
}

void Shader::init()
{
    // 2. compile shaders
    unsigned int vid, fid;
    int success;
    char infoLog[512];
    const char *vs = vertex.c_str();
    const char *fs = fragment.c_str();
    const char *gs = geometry.c_str();
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
    if (geometry != "")
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
    if (geometry != "")
        glAttachShader(ID, gid);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vid);
    glDeleteShader(fid);
    if (geometry != "")
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
    if(ID == 0)
    {
        init();
    }
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
            LOG(ERROR) << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                       << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            LOG(ERROR) << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                       << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
}