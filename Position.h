#pragma once

class Position {
private:
    int row;
    int column;
public:
    Position(int row, int column);
    ~Position();
    int getRow() const;
    int getColumn() const;
    std::pair<int,int> getCoordinate() const;
    void setCoordinate(int row, int column);
    bool operator== (const Position& obj);
};