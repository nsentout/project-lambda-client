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
    int m_red_offset;

    Position m_position;
    float m_size;
    Color m_color;

    unsigned int m_vbo;
    unsigned int m_vao;
    unsigned int m_ebo;

    Shader *m_shader;

public:
    GameObject();
    ~GameObject();

    virtual void init() = 0;
    virtual void draw() const = 0;
    virtual void erase() const = 0;

    virtual void moveX(unsigned int movement) = 0;
    virtual void moveY(unsigned int movement) = 0;
    virtual void setXY(int x, int y) = 0;
    virtual void setColor(Color color) = 0;
    int getX() const;
    int getY() const;
    Color getColor() const;
};

#endif