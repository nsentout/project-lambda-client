#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Position.h"
#include "Color.h"

class Triangle
{
private:
    GLfloat *m_vertices;
    size_t m_vertices_size;
    int m_nb_vertices;

    int m_stride;
    int m_x_offset;
    int m_y_offset;

    unsigned int m_vbo;
    unsigned int m_vao;
    unsigned int m_program_id;

    void compileShaders(const char *vertexPath, const char *fragmentPath);

public:
    Triangle();
    ~Triangle();

    void init();
    void draw() const;

    void moveX(float movement);
    void moveY(float movement);
    void setXY(float x, float y);
};

#endif