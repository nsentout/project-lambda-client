#ifndef SHADERREGISTRY_HPP
#define SHADERREGISTRY_HPP

#include "render/Shader.hpp"
#include "const.h"

#include <vector>

#define BASIC_SHADER_INDEX 0

class ShaderRegistry
{
private:
    static ShaderRegistry *m_instance;
    std::vector<Shader*> m_shaders;

public:
    ShaderRegistry();
    ~ShaderRegistry();

    Shader* getBasicShader() const;

    static ShaderRegistry* getInstance();
};


#endif