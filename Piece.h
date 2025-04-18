#pragma once

#include "Enums.h"
#include "Position.h"
#include <vector>
#include <string>

class Position;
class Board;

class Piece {
protected:
    Color color;
    Position position;
    bool isCaptured;
    char character;
    std::string symbolForBoard;
public:
    Piece(Color color, const char character, const Position& position);
    virtual ~Piece();
    Color getColor() const;
    Position getPosition() const;
    void setPosition(int row, int col);
    bool getisCaptured() const;
    char getPieceCharacter() const;
    void setisCaptured(bool captured);
    bool canMove(const Position& to, Board* board) const;
    virtual std::vector<Position> getValidMoves(Board* board) const = 0;
    std::string getPieceSymbol(char piece) const;
    std::string getSymbolForBoard() const;
    bool isInBounds(const Position& pos) const;
    bool isInBounds(int row, int column) const;
};