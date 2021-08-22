#include "render/Triangle.hpp"
#include "render/ShaderRegistry.hpp"

#include <stdlib.h>
#include <string.h>

Triangle::Triangle()
{
    m_nb_vertices = 18;
    m_stride = 6;
    m_x_offset = 0;
    m_y_offset = 1;

    m_vertices = (GLfloat*) malloc(sizeof(GLfloat) * m_nb_vertices);
    m_vertices_size = sizeof(GLfloat) * m_nb_vertices;

    GLfloat default_vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    memcpy(m_vertices, default_vertices, sizeof(default_vertices));

    m_shader = ShaderRegistry::getInstance()->getBasicShader();
}

void Triangle::init()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, m_vertices_size, m_vertices, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Triangle::draw() const
{
    m_shader->use();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::erase() const
{

}

void Triangle::moveX(unsigned int movement)
{
    m_position.x += movement;

    m_vertices[m_x_offset] += movement;
    m_vertices[m_x_offset + m_stride] += movement;
    m_vertices[m_x_offset + m_stride * 2] += movement;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}

void Triangle::moveY(unsigned int movement)
{
    m_position.y += movement;

    m_vertices[m_y_offset] += movement;
    m_vertices[m_y_offset + m_stride] += movement;
    m_vertices[m_y_offset + m_stride * 2] += movement;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}

void Triangle::setXY(int x, int y)
{
    m_position.x = x;
    m_position.y = y;

    float normalized_x =  x / SCREEN_WIDTH;
    float normalized_y =  (-y) / SCREEN_HEIGHT;

    m_vertices[m_x_offset] = normalized_x + 0.5f;
    m_vertices[m_x_offset + m_stride] = normalized_x - 0.5f;
    m_vertices[m_x_offset + m_stride * 2] = normalized_x;

    m_vertices[m_y_offset] = normalized_y - 1.0f;
    m_vertices[m_y_offset + m_stride] = normalized_y - 1.0f;
    m_vertices[m_y_offset + m_stride * 2] = normalized_y;
 
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}
