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
    char character; // N , K , Q , B , R , P // black pieces are lower case
    // white pieces are upper case
    std::string symbolForBoard;
    PieceType type;
public:
    Piece(Color color, const char character, const Position& position);
    virtual ~Piece();
    Color getColor() const;
    Position getPosition() const;
    void setPosition(const int row, const int col);
    void setPosition(const Position& position);
    void setType(PieceType type);
    PieceType getType() const;
    bool getisCaptured() const;
    char getPieceCharacter() const;
    void setisCaptured(bool captured);
    bool canMove(const Position& to, Board* board) const;
    bool isPiecePinnedDiagonally(Board* board) const;
    bool isPiecePinnedHorizontally(Board* board) const;
    bool isPiecePinnedVertically(Board* board) const;
    virtual std::vector<Position> getValidMoves(Board* board) const = 0;
    virtual bool canAttack(const Position& to, Board* board) const = 0;
    static std::string getPieceSymbol(char piece);
    std::string getSymbolForBoard() const;
    bool isInBounds(const Position& pos) const;
    bool isInBounds(int row, int column) const;
};