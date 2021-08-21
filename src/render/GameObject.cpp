#include "render/GameObject.hpp"

const Color DEFAULT_COLOR = {255, 0, 0, 255};

GameObject::GameObject() : GameObject(DEFAULT_COLOR)
{
    
}

GameObject::GameObject(Color color) 
{
    m_color = color;
}


GameObject::~GameObject()
{
    free(m_vertices);

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}
