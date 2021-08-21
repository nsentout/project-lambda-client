#include "render/Rectangle.hpp"

#include <stdlib.h>
#include <string.h>
#include <iostream>

const float DEFAULT_WIDTH = 400.0f;
const float DEFAULT_HEIGHT = 400.0f;

Rectangle::Rectangle() : Rectangle(DEFAULT_WIDTH, DEFAULT_HEIGHT)
{
    
}

Rectangle::Rectangle(float width, float height)
{
    m_nb_vertices = 24;
    m_stride = 6;
    m_x_offset = 0;
    m_y_offset = 1;
    //m_width = width / (SCREEN_WIDTH / 2);
    //m_height = height / (SCREEN_HEIGHT / 2);
    m_width = width;
    m_height = height;

    m_vertices = (GLfloat*) malloc(sizeof(GLfloat) * m_nb_vertices);
    m_vertices_size = sizeof(GLfloat) * m_nb_vertices;

    GLfloat default_vertices[] = {
        // positions         // colors
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // bottom right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // top right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   // top left
        -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,   // bottom left
    };
    m_color = { 255, 0, 0, 0 };
    GLfloat r = m_color.r / 255;
    GLfloat g = m_color.g / 255;
    GLfloat b = m_color.b / 255;
    GLfloat a = m_color.a / 255;

    /*GLfloat default_vertices[] = {
        // positions         // colors
        0.5f, 0.5f, 0.0f,   r, g, b,   // bottom right
        0.5f, -0.5f, 0.0f,  r, g, b,   // top right
        -0.5f, -0.5f, 0.0f, r, g, b,   // top left
        -0.5f, 0.5f, 0.0f,  r, g, b,   // bottom left
    };*/

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
    //glScissor()
}

void Rectangle::moveX(float movement)
{
    m_vertices[m_x_offset] += movement;
    m_vertices[m_x_offset + m_stride] += movement;
    m_vertices[m_x_offset + m_stride * 2] += movement;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}

void Rectangle::moveY(float movement)
{
    m_vertices[m_y_offset] += movement;
    m_vertices[m_y_offset + m_stride] += movement;
    m_vertices[m_y_offset + m_stride * 2] += movement;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}

void Rectangle::setXY(float x, float y)
{
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

float Rectangle::getX()
{
    return 0.0f;
}

float Rectangle::getY()
{
    return 0.0f;
}