#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "render/GameObject.hpp"
#include "render/ShaderRegistry.hpp"

#include "Position.h"
#include "Color.h"

class Rectangle : public GameObject
{
private:
    float m_width;
    float m_height;
public:
    Rectangle();
    Rectangle(float width, float height);
    Rectangle(float width, float height, Color color);

    void init() override;
    void draw() const override;
    void erase() const override;

    void moveX(unsigned int movement) override;
    void moveY(unsigned int movement) override;
    void setXY(int x, int y) override;
    void setColor(Color color) override;
};

#endif