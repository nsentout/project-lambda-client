#include "render/Triangle.hpp"

#include <stdlib.h>
#include <string.h>
#include "const.h"
#include <iostream>
#include <fstream>
#include <sstream>

const char *vertexShaderPath = "shaders/shader.vs";
const char *fragmentShaderPath = "shaders/shader.fs";

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
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    memcpy(m_vertices, default_vertices, sizeof(default_vertices));
}

Triangle::~Triangle()
{
    free(m_vertices);

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
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

    compileShaders(vertexShaderPath, fragmentShaderPath);
}

void Triangle::draw() const
{
    glUseProgram(m_program_id);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glBindVertexArray(0);
}

void Triangle::moveX(float movement)
{
    m_vertices[m_x_offset] += movement;
    m_vertices[m_x_offset + m_stride] += movement;
    m_vertices[m_x_offset + m_stride * 2] += movement;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}

void Triangle::moveY(float movement)
{
    m_vertices[m_y_offset] += movement;
    m_vertices[m_y_offset + m_stride] += movement;
    m_vertices[m_y_offset + m_stride * 2] += movement;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices_size, m_vertices);
}

void Triangle::setXY(float x, float y)
{
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

void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Triangle::compileShaders(const char *vertexPath, const char *fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    m_program_id = glCreateProgram();
    glAttachShader(m_program_id, vertex);
    glAttachShader(m_program_id, fragment);
    glLinkProgram(m_program_id);
    checkCompileErrors(m_program_id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
