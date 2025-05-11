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
    Piece* getPiece(int row, int column) const;
    Piece* getPiece(const Position& position) const;
    void movePiece(Piece *piece, const Position& position, Player& player);
    void movePieceTemp(Piece *piece, const Position& position);
    void removePiece(const Position& position);
    void printBoard() const;
    bool isSquareOccupied(const Position& position) const;
    std::vector<Piece*> getAllPieces() const;
    std::vector<Piece*> getAllPiecesOfColor(Color color) const;
    Position getKingPosition(Color color) const;
};