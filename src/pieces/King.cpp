#include <iostream>
#include <vector>
#include "../../include/pieces/Piece.h"
#include "../../include/pieces/King.h"
#include "../../include/core/Position.h"
#include "../../include/core/Board.h"
#include "../../include/pieces/Rook.h"
#include "../../include/Enums.h"
#include "../../include/utils/GlobalVariables.h"

King::King(Color color, const char character, const Position& position):
Piece(color, character, position), hasMoved(false) {}

King::~King() {}

std::vector<Position> King::getValidMoves(Board* board) const {
    std::vector<Position> ValidMoves;
    std::vector<Piece*> AllPieces = board->getAllPiecesOfColor(color == Color::White ? Color::Black : Color::White);
    int row = position.X;
    int col = position.Y;
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };
    for (int i = 0; i < 8; i++) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        Position newPosition(newRow, newCol);
        if (!isInBounds(newRow, newCol)) {
            continue;
        }
        bool flag = false;
        for(Piece* piece: AllPieces) {
            if(piece->canAttack(newPosition, board)) {
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

bool King::canCastle() const {
    return !hasMoved;
}

bool King::canAttack(const Position& to, Board* board) const {
    int row = position.X;
    int col = position.Y;
    
    // Properly defined directions array
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };
    
    // Correct iteration over the directions array
    for (int i = 0; i < 8; i++) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        
        // Skip if out of bounds
        if (!isInBounds(newRow, newCol)) {
            continue;
        }

        Position newPosition(newRow, newCol);
        Piece* piece = board->getPiece(newPosition);
        
        // Check if position is attackable (empty or has opponent piece)
        if (!piece || piece->getColor() != color) {
            // Direct check against target position instead of storing in vector
            if (newPosition == to) {
                return true;
            }
        }
    }
    
    return false;
}

