#pragma once

#include <vector>
#include "Square.h"
#include "Position.h"

class Piece;

class Board {
private:
    std::vector<std::vector<Square>> squares;
public:
    Board(std::vector<std::vector<Square>>& squares);
    ~Board();
    Piece* getPiece(const Position& position) const;
    void placePiece(Piece *piece, const Position& position);
    void removePiece(const Position& position);
    bool isSquareOccupied(const Position& position) const;
    std::vector<Piece*> getAllPieces() const;
};