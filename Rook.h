#pragma once

#include "Piece.h"
#include "Position.h"
#include "Board.h"
#include "Enums.h"

class Rook: public Piece {
private:
    bool hasMoved;
public:
    Rook(Color color, const char character, const Position& position);
    ~Rook();
    void setHasMoved(bool moved);
    bool getHasMoved() const;
    std::vector<Position> getValidMoves(Board* board) const override;
};