#include "render/Rectangle.hpp"

#include <stdlib.h>
#include <string.h>
#include <iostream>

const float DEFAULT_WIDTH = 400.0f;
const float DEFAULT_HEIGHT = 400.0f;
const Color DEFAULT_COLOR = { 150, 150, 150, 0 };

Rectangle::Rectangle() : Rectangle(DEFAULT_WIDTH, DEFAULT_HEIGHT)
{ }

Rectangle::Rectangle(float width, float height) : Rectangle(width, height, DEFAULT_COLOR)
{ }

Rectangle::Rectangle(float width, float height, Color color)
{
    m_nb_vertices = 24;
    m_stride = 6;
    m_x_offset = 0;
    m_y_offset = 1;
    m_red_offset = 3;
    m_position = { 0, 0 };
    m_width = width;
    m_height = height;
    m_color = color;

    m_vertices = (GLfloat*) malloc(sizeof(GLfloat) * m_nb_vertices);
    m_vertices_size = sizeof(GLfloat) * m_nb_vertices;

    GLfloat r = m_color.r / 255.0f;
    GLfloat g = m_color.g / 255.0f;
    GLfloat b = m_color.b / 255.0f;

    GLfloat default_vertices[] = {
        // positions               // colors
        m_width, 0.0f,     0.0f,   r, g, b,   // top right
        m_width, m_height, 0.0f,   r, g, b,   // bottom right
        0.0f,    m_height, 0.0f,   r, g, b,   // bottom left
        0.0f,    0.0f,     0.0f,   r, g, b,   // top left
    };

    memcpy(m_vertices, default_vertices, sizeof(default_vertices));

    m_shader = ShaderRegistry::getInstance()->getBasicShader();
}
    

void Rectangle::init()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, m_vertices_size, m_vertices, GL_DYNAMIC_DRAW);
    
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Rectangle::draw() const
{
    m_shader->use();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Rectangle::erase() const
{
    glScissor(m_position.x, m_position.y, m_width, m_height);
}

void Rectangle::moveX(unsigned int movement)
{
    m_position.x += movement;

    m_vertices[m_x_offset] += movement;
    m_vertices[m_x_offset + m_stride] += movement;
    m_vertices[m_x_offset + m_stride * 2] += movement;
    m_vertices[m_x_offset + m_stride * 3] += movement;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}

void Rectangle::moveY(unsigned int movement)
{
    m_position.y += movement;

    m_vertices[m_y_offset] += movement;
    m_vertices[m_y_offset + m_stride] += movement;
    m_vertices[m_y_offset + m_stride * 2] += movement;
    m_vertices[m_y_offset + m_stride * 3] += movement;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}

void Rectangle::setXY(int x, int y)
{
    m_position.x = x;
    m_position.y = y;

    m_vertices[m_x_offset] = x + m_width;
    m_vertices[m_x_offset + m_stride] = x + m_width;
    m_vertices[m_x_offset + m_stride * 2] = x;
    m_vertices[m_x_offset + m_stride * 3] = x;

    m_vertices[m_y_offset] = y;
    m_vertices[m_y_offset + m_stride] = y + m_height;
    m_vertices[m_y_offset + m_stride * 2] = y + m_height;
    m_vertices[m_y_offset + m_stride * 3] = y;
 
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}

void Rectangle::setColor(Color color)
{
    m_color = color;

    for (int i = m_red_offset; i < m_nb_vertices; i+= m_stride) {
        m_vertices[i]     = color.r;
        m_vertices[i + 1] = color.g;
        m_vertices[i + 2] = color.b;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}
