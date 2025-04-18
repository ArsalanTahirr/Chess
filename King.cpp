#include <iostream>
#include <vector>
#include "Piece.h"
#include "King.h"
#include "Position.h"
#include "Board.h"
#include "GlobalVariables.h"

King::King(Color color, const char character, const Position& position):
Piece(color, character, position), hasMoved(false) {}

King::~King() {}

std::vector<Position> King::getValidMoves(Board* board) const {
    std::vector<Position> ValidMoves;
    std::vector<Piece*> AllPieces = board->getAllPieces();
    int row = position.getRow();
    int col = position.getColumn();
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
            if(piece->canMove(newPosition, board) && piece->getColor() != color) {
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

bool King::canCastle() const {
    return !hasMoved;
}