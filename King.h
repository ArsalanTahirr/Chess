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
    bool canCastle() const;
    bool canAttack(const Position& to, Board* board) const override;
};