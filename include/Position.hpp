#ifndef POSITION_HPP
#define POSITION_HPP

class Position
{
private:
    int m_x;
    int m_y;

public:
    Position();
    Position(int x, int y);

    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
};

#endif