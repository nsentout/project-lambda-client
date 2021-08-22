#include "render/GameObject.hpp"

const Color DEFAULT_COLOR = {255, 0, 0, 255};

GameObject::GameObject()
{ }

GameObject::~GameObject()
{
    free(m_vertices);

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

int GameObject::getX() const
{
    return m_position.x;
}

int GameObject::getY() const
{
    return m_position.y;
}

Color GameObject::getColor() const
{
    return m_color;
}