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

    void init() override;
    void draw() const override;
    void erase() const override;

    void moveX(float movement) override;
    void moveY(float movement) override;
    void setXY(float x, float y) override;

    float getX() override;
    float getY() override;
};

#endif