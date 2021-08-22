#include "render/ShaderRegistry.hpp"

#define NOTEXTURE_SHADER_INDEX 0
#define GAMEOBJECT_SHADER_INDEX 1

const char *NOTEXTURE_VERTEX_SHADER_PATH = "shaders/gameobject_notexture.vs";
const char *NOTEXTURE_FRAGMENT_SHADER_PATH = "shaders/gameobject_notexture.fs";

const char *GAMEOBJECT_VERTEX_SHADER_PATH = "shaders/gameobject.vs";
const char *GAMEOBJECT_FRAGMENT_SHADER_PATH = "shaders/gameobject.fs";

ShaderRegistry *ShaderRegistry::m_instance = nullptr;

ShaderRegistry::ShaderRegistry()
{
    Shader *notexture_shader = new Shader();
    notexture_shader->compileShaders(NOTEXTURE_VERTEX_SHADER_PATH, NOTEXTURE_FRAGMENT_SHADER_PATH);
    m_shaders.push_back(notexture_shader);

    Shader *gameobject_shader = new Shader();
    gameobject_shader->compileShaders(GAMEOBJECT_VERTEX_SHADER_PATH, GAMEOBJECT_FRAGMENT_SHADER_PATH);
    m_shaders.push_back(gameobject_shader);
}

ShaderRegistry::~ShaderRegistry()
{
    for (Shader* shader : m_shaders) {
        delete shader;
    }
}

Shader* ShaderRegistry::getNoTextureShader() const
{
    return m_shaders[NOTEXTURE_SHADER_INDEX];
}

Shader* ShaderRegistry::getGameObjectShader() const
{
    return m_shaders[GAMEOBJECT_SHADER_INDEX];
}

ShaderRegistry* ShaderRegistry::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new ShaderRegistry();
    }
    return m_instance;
}