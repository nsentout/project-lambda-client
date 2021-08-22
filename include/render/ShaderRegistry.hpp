#ifndef SHADERREGISTRY_HPP
#define SHADERREGISTRY_HPP

#include "render/Shader.hpp"
#include "const.h"

#include <vector>

class ShaderRegistry
{
private:
    static ShaderRegistry *m_instance;
    std::vector<Shader*> m_shaders;

public:
    ShaderRegistry();
    ~ShaderRegistry();

    Shader* getNoTextureShader() const;
    Shader* getGameObjectShader() const;

    static ShaderRegistry* getInstance();
};


#endif