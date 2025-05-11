#pragma once

#include "Piece.h"

class Position;
class Board;

class King: public Piece {
private:
    bool hasMoved;
public:
    King(Color color, const char character, const Position& position);
    ~King();
    void setHasMoved(bool moved);
    bool getHasMoved() const;
    std::vector<Position> getValidMoves(Board* board) const override;
    bool canCastleKS(Board* board) const;
    bool canCastleQS(Board* board) const;
    bool canAttack(const Position& to, Board* board) const override;
};