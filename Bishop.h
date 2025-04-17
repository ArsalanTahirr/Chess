#pragma once

#include "Piece.h"
#include "Position.h"
#include "Board.h"
#include "Enums.h"

class Bishop: public Piece {
public:
    Bishop(Color color, const char character, const Position& position);
    ~Bishop();
    std::vector<Position> getValidMoves(Board* board) const override;
};