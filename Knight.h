#pragma once

#include "Piece.h"
#include "Position.h"
#include "Board.h"
#include "Enums.h"

class Knight: public Piece {
public:
    Knight(Color color, const char character, const Position& position);
    ~Knight();
    std::vector<Position> getValidMoves(Board* board) const override;
};