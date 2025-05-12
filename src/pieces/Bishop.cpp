#include <iostream>
#include "../../include/pieces/Bishop.h"
#include "../../include/utils/GlobalVariables.h"

Bishop::Bishop(Color color, const char character, const Position& position):
Piece(color, character, position) {}

Bishop::~Bishop() {}

std::vector<Position> Bishop::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    int row = position.X;
    int col = position.Y;

    int directions[4][2];
    int ROWS = 0;

    if (isPiecePinnedDiagonally(board)) {
        int directionX = (board->getKingPosition(getColor()).X > position.X) ? 1 : -1;
        int directionY = (board->getKingPosition(getColor()).Y > position.Y) ? 1 : -1;
        directions[0][0] = directionX;  directions[0][1] = directionY;
        directions[1][0] = -directionX; directions[1][1] = -directionY;
        ROWS = 2;
    } else if (isPiecePinnedHorizontally(board) || isPiecePinnedVertically(board)) {
        return validMoves; // No valid moves if pinned horizontally or vertically
    } else {
        directions[0][0] = -1; directions[0][1] = -1;
        directions[1][0] = 1;  directions[1][1] = 1;
        directions[2][0] = 1;  directions[2][1] = -1;
        directions[3][0] = -1; directions[3][1] = 1;
        ROWS = 4;
    }

    for (int i = 0; i < ROWS; i++) {
        int* dir = directions[i];
        int newRow = row;
        int newCol = col;
        while (true) {
            newRow += dir[0];
            newCol += dir[1];
            if (!isInBounds(newRow, newCol)) break;
            Position newPos(newRow, newCol);
            Piece* pieceAtNewPos = board->getPiece(newPos);
            if (pieceAtNewPos == nullptr) {
                validMoves.push_back(newPos);
            } else if (pieceAtNewPos->getColor() != color) {
                validMoves.push_back(newPos);
                break;
            } else {
                break;
            }
        }
    }
    return validMoves;
}

bool Bishop::canAttack(const Position& to, Board* board) const {
    int row = position.X;
    int col = position.Y;

    int directions[4][2] = {
        {-1, -1}, {1, 1}, {1, -1}, {-1, 1}
    };
    
    for (int i = 0; i < 4; i++) {
        int newRow = row;
        int newCol = col;
        
        while (true) {
            newRow += directions[i][0];
            newCol += directions[i][1];

            if (!isInBounds(newRow, newCol)) break;
            
            Position newPos(newRow, newCol);
            
            // If this is the target position, we can return immediately
            if (newPos == to) {
                Piece* pieceAtNewPos = board->getPiece(newPos);
                // Can attack if square is empty or has opponent's piece
                if (pieceAtNewPos == nullptr || pieceAtNewPos->getColor() != color) {
                    return true;
                }
                break;
            }
            
            Piece* pieceAtNewPos = board->getPiece(newPos);
            
            // If there's a piece blocking the path
            if (pieceAtNewPos != nullptr) {
                // We're blocked in this direction
                break;
            }
        }
    }
    
    return false;
}

