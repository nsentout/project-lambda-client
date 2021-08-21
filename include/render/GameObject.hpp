#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Position.h"
#include "Color.h"
#include "Shader.hpp"

class GameObject
{
protected:
    GLfloat *m_vertices;
    size_t m_vertices_size;
    int m_nb_vertices;

    int m_stride;
    int m_x_offset;
    int m_y_offset;

    Position position;
    float m_size;
    Color m_color;

    unsigned int m_vbo;
    unsigned int m_vao;
    unsigned int m_ebo;

    Shader *m_shader;

public:
    GameObject();
    GameObject(Color color);
    ~GameObject();

    virtual void init() = 0;
    virtual void draw() const = 0;
    virtual void erase() const = 0;

    virtual void moveX(float movement) = 0;
    virtual void moveY(float movement) = 0;
    virtual void setXY(float x, float y) = 0;
    virtual float getX() = 0;
    virtual float getY() = 0;
};

#endif