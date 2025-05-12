#pragma once

class Position {
public:
    int X;
    int Y;
    Position();
    Position(const int X, const int Y);
    ~Position();
    void setCoordinate(const Position& obj);
    void setCoordinate(const int X, const int Y);
    bool operator== (const Position& obj) const;
    bool operator!= (const Position& obj) const;
};
