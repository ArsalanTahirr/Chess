#pragma once

#include "Piece.h"

class Position;
class Board;

class Queen: public Piece {
public:
    Queen(Color color, const char character, const Position& position);
    ~Queen();
    std::vector<Position> getValidMoves(Board* board) const override;
};