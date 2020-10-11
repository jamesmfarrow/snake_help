#ifndef COORDINATE_H
#define COORDINATE_H


class coordinate
{
 public:
    int m_x;
    int m_y;

    coordinate(int x=0, int y=0) : m_x(x), m_y(y) {}

    //copy constructor
    coordinate(const coordinate &other) {
        m_x = other.m_x;
        m_y = other.m_y;
    }

    //assignment overload operator
    coordinate& operator=(const coordinate &coord) {
        m_x = coord.m_x;
        m_y = coord.m_y;
        return *this;
    }

    bool operator==(const coordinate& rhs) {
        return this->m_x == rhs.m_x && this->m_y == rhs.m_y;
    }


};


#endif // COORDINATE_H
