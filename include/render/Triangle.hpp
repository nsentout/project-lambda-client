#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameObject.hpp"
#include "Position.h"
#include "Color.h"

class Triangle : public GameObject
{
public:
    Triangle();

    void init() override;
    void draw() const override;
    void erase() const override;

    void moveX(unsigned int movement) override;
    void moveY(unsigned int movement) override;
    void setXY(int x, int y) override;
};

#endif