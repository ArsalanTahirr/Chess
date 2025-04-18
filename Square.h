#pragma once

#include "Enums.h"
#include "Position.h"

class Piece;
class Board;

class Square {
private:
    Position position;
    Piece *piece;
    Color color;
    Square();
public:
    Square(const Position& position, Piece *piece);
    ~Square();
    Piece* getPiece() const;
    void setPiece(Piece *piece);
    bool isEmpty() const;
    Position getPosition() const;
    Color getColor() const;
    friend class Board; // Granting access to class Board
};