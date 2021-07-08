#include "Position.hpp"

Position::Position() 
{
    
}


Position::Position(int x, int y) : m_x(x), m_y(y)
{
    
}

int Position::getX() const
{
    return m_x;
}

int Position::getY() const
{
    return m_y;
}

void Position::setX(int x) 
{
    m_x = x;
}

void Position::setY(int y) 
{
    m_y = y;
}
