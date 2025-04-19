#pragma once

#include "Piece.h"
#include "Position.h"
#include "Board.h"
#include "Enums.h"

class Pawn: public Piece {
private:
    bool hasMoved;
public:
    Pawn(Color color, const char character, const Position& position);
    ~Pawn();
    void setHasMoved(bool moved);
    bool getHasMoved() const;
    std::vector<Position> getValidMoves(Board* board) const override;
};