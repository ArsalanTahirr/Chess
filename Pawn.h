#pragma once

#include "Piece.h"
#include "Position.h"
#include "Board.h"
#include "Enums.h"

class Pawn: public Piece {
private:
    bool hasMoved;
    bool canCaptureWithEnPassant;
public:
    Pawn(Color color, const char character, const Position& position);
    ~Pawn();
    void setHasMoved(bool moved);
    void setCanCaptureWithEnPassant(bool canCapture);
    bool getHasMoved() const;
    bool getCanCaptureWithEnPassant() const;
    std::vector<Position> getValidMoves(Board* board) const override;
    bool canAttack(const Position& to, Board* board) const override;
};