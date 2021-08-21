#include "render/ShaderRegistry.hpp"

const char *VERTEX_SHADER_PATH = "shaders/gameobject.vs";
const char *FRAGMENT_SHADER_PATH = "shaders/gameobject.fs";

ShaderRegistry *ShaderRegistry::m_instance = nullptr;

ShaderRegistry::ShaderRegistry()
{
    Shader *basic_shader = new Shader();
    basic_shader->compileShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    m_shaders.push_back(basic_shader);
}

ShaderRegistry::~ShaderRegistry()
{
    for (Shader* shader : m_shaders) {
        delete shader;
    }
}

Shader* ShaderRegistry::getBasicShader() const
{
    return m_shaders[BASIC_SHADER_INDEX];
}

ShaderRegistry* ShaderRegistry::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new ShaderRegistry();
    }
    return m_instance;
}