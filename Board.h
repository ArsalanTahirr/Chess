#pragma once

#include <vector>
#include "Square.h"
#include "Position.h"
#include <windows.h>

class Piece;
class Player;

class Board {
private:
    Square squares[8][8];
    void setColor(HANDLE console, bool isWhiteSquare, bool isWhitePiece, bool hasPiece) const;
    void resetColor(HANDLE console) const;
public:
    Board();
    ~Board();
    void initializeBoard();
    Piece* getPiece(const Position& position) const;
    void placePiece(Piece *piece, const Position& position, Player& player);
    void removePiece(const Position& position);
    void printBoard() const;
    bool isSquareOccupied(const Position& position) const;
    std::vector<Piece*> getAllPieces() const;
};