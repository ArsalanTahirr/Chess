#include <iostream>
#include <vector>
#include "Piece.h"
#include "King.h"
#include "Position.h"
#include "Board.h"
#include "Rook.h"
#include "GlobalVariables.h"

King::King(Color color, const char character, const Position& position):
Piece(color, character, position), hasMoved(false) {}

King::~King() {}

std::vector<Position> King::getValidMoves(Board* board) const {
    std::vector<Position> ValidMoves;
    std::vector<Piece*> AllPieces = board->getAllPieces();
    int row = position.X;
    int col = position.Y;
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };
    for (int *dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];
        Position newPosition(newRow, newCol);
        if (!isInBounds(newRow, newCol)) {
            continue;
        }
        bool flag = false;
        for(Piece* piece: AllPieces) {
            if(piece->canAttack(newPosition, board) && piece->getColor() != color) {
                flag = true;
                break;
            }
        }
        if(flag) {
            continue;
        }
        Piece *piece = board->getPiece(newPosition);
        if (!piece || piece->getColor() != color) {
            ValidMoves.push_back(newPosition);
        }
    }
    return ValidMoves;
}

void King::setHasMoved(bool moved) {
    hasMoved = moved;
}

bool King::getHasMoved() const {
    return hasMoved;
}

bool King::canCastleKS(Board* board) const {
    Piece* rook;
    if(color == Color::White) {
        rook = board->getPiece(Position(0, 7));
    } else {
        rook = board->getPiece(Position(7, 7));
    }
    Rook* rookPtr = dynamic_cast<Rook*>(rook);
    return !hasMoved && rookPtr != nullptr && rookPtr->getHasMoved() == false;
}

bool King::canCastleQS(Board* board) const {
    Piece* rook;
    if(color == Color::White) {
        rook = board->getPiece(Position(0, 0));
    } else {
        rook = board->getPiece(Position(0, 7));
    }
    Rook* rookPtr = dynamic_cast<Rook*>(rook);
    return !hasMoved && rookPtr != nullptr && rookPtr->getHasMoved() == false;
}

bool King::canAttack(const Position& to, Board* board) const {
    std::vector<Position> ValidMoves;
    std::vector<Piece*> AllPieces = board->getAllPieces();
    int row = position.X;
    int col = position.Y;
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };
    for (int *dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];
        Position newPosition(newRow, newCol);
        if (!isInBounds(newRow, newCol)) {
            continue;
        }

        Piece *piece = board->getPiece(newPosition);
        if (!piece || piece->getColor() != color) {
            ValidMoves.push_back(newPosition);
        }
    }
    for (const Position& move : ValidMoves) {
        if (to == move) {
            return true;
        }
    }
    return false;
}