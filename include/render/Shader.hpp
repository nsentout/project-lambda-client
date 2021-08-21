#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>

class Shader
{
private:
    unsigned int m_program_id;

public:
    void use() const;
    void compileShaders(const char *vertexPath, const char *fragmentPath);

    int getUniformLocation(const char* uniform_name) const;
    void setUniformMatrix4fv(int location, const glm::mat4 &matrix) const;
};


#endif