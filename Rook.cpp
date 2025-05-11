#include <iostream>
#include "Rook.h"
#include "GlobalVariables.h"

Rook::Rook(Color color, const char character, const Position& position):
Piece(color, character, position), hasMoved(false) {}

Rook::~Rook() {}

void Rook::setHasMoved(bool moved) {
    hasMoved = moved;
}

bool Rook::getHasMoved() const {
    return hasMoved;
}

std::vector<Position> Rook::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    int row = position.X;
    int col = position.Y;
    
    // Use stack array instead of dynamic allocation
    int directions[4][2];
    int ROWS = 0;

    if(isPiecePinnedDiagonally(board)) {
        return validMoves; // No valid moves if pinned diagonally
    }
    else if(isPiecePinnedHorizontally(board)) {
        int directionX = (board->getKingPosition(getColor()).X > position.X) ? 1 : -1;
        directions[0][0] = directionX;  directions[0][1] = 0;
        directions[1][0] = -directionX; directions[1][1] = 0;
        ROWS = 2;
    }
    else if(isPiecePinnedVertically(board)) {
        int directionY = (board->getKingPosition(getColor()).Y > position.Y) ? 1 : -1;
        directions[0][0] = 0; directions[0][1] = directionY;
        directions[1][0] = 0; directions[1][1] = -directionY;
        ROWS = 2;
    }
    else {
        directions[0][0] = -1; directions[0][1] = 0;  // Up
        directions[1][0] = 1;  directions[1][1] = 0;  // Down
        directions[2][0] = 0;  directions[2][1] = -1; // Left
        directions[3][0] = 0;  directions[3][1] = 1;  // Right
        ROWS = 4;
    }

    for (int i = 0; i < ROWS; i++) {
        int newRow = row;
        int newCol = col;
        
        while (true) {
            newRow += directions[i][0];
            newCol += directions[i][1];
            
            if (!isInBounds(newRow, newCol)) break;
            
            Position newPos(newRow, newCol);
            Piece* pieceAtNewPos = board->getPiece(newPos);
            
            if (pieceAtNewPos == nullptr) {
                validMoves.push_back(newPos);
            } 
            else if (pieceAtNewPos->getColor() != color) {
                validMoves.push_back(newPos);
                break;
            } 
            else {
                break;
            }
        }
    }
    
    return validMoves;
}

bool Rook::canAttack(const Position& to, Board* board) const {
    int row = position.X;
    int col = position.Y;
    
    // Rook moves in 4 directions (horizontal and vertical)
    const int directions[4][2] = {
        {-1, 0},  // Up
        {1, 0},   // Down
        {0, -1},  // Left
        {0, 1}    // Right
    };

    for (int i = 0; i < 4; i++) {
        int newRow = row;
        int newCol = col;
        
        while (true) {
            newRow += directions[i][0];
            newCol += directions[i][1];
            
            if (!isInBounds(newRow, newCol)) break;
            
            Position newPos(newRow, newCol);
            
            // If this is the target position
            if (newPos == to) {
                Piece* pieceAtNewPos = board->getPiece(newPos);
                // Can attack if square is empty or has opponent's piece
                if (pieceAtNewPos == nullptr || pieceAtNewPos->getColor() != color) {
                    return true;
                }
                break;
            }
            
            Piece* pieceAtNewPos = board->getPiece(newPos);
            
            // If there's a piece in the way (blocking), stop checking this direction
            if (pieceAtNewPos != nullptr) {
                break;
            }
        }
    }
    
    return false;
}