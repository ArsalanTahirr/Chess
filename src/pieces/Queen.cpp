#include <iostream>
#include <vector>
#include "../../include/pieces/Piece.h"
#include "../../include/pieces/Queen.h"
#include "../../include/core/Board.h"
#include "../../include/core/Position.h"
#include "../../include/utils/GlobalVariables.h"

Queen::Queen(Color color, const char character, const Position& position):
Piece(color, character, position) {}

Queen::~Queen() {}

std::vector<Position> Queen::getValidMoves(Board* board) const {
    std::vector<Position> validMoves;
    int row = position.X;
    int col = position.Y;

    int directions[8][2];
    int ROWS = 0;

    if (isPiecePinnedDiagonally(board)) {
        int directionX = (board->getKingPosition(getColor()).X > position.X) ? 1 : -1;
        int directionY = (board->getKingPosition(getColor()).Y > position.Y) ? 1 : -1;
        directions[0][0] = directionX;  directions[0][1] = directionY;
        directions[1][0] = -directionX; directions[1][1] = -directionY;
        ROWS = 2;
    } else if (isPiecePinnedHorizontally(board)) {
        int directionX = (board->getKingPosition(getColor()).X > position.X) ? 1 : -1;
        directions[0][0] = directionX; directions[0][1] = 0;
        directions[1][0] = -directionX; directions[1][1] = 0;
        ROWS = 2;
    } else if (isPiecePinnedVertically(board)) {
        int directionY = (board->getKingPosition(getColor()).Y > position.Y) ? 1 : -1;
        directions[0][0] = 0; directions[0][1] = directionY;
        directions[1][0] = 0; directions[1][1] = -directionY;
        ROWS = 2;
    } else {
        directions[0][0] = 0;  directions[0][1] = 1;   // Up
        directions[1][0] = 0;  directions[1][1] = -1;  // Down
        directions[2][0] = -1; directions[2][1] = 0;   // Left
        directions[3][0] = 1;  directions[3][1] = 0;   // Right
        directions[4][0] = -1; directions[4][1] = 1;   // Up-Left
        directions[5][0] = 1;  directions[5][1] = 1;   // Up-Right
        directions[6][0] = -1; directions[6][1] = -1;  // Down-Left
        directions[7][0] = 1;  directions[7][1] = -1;  // Down-Right
        ROWS = 8;
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

bool Queen::canAttack(const Position& to, Board* board) const {
    int row = position.X;
    int col = position.Y;
    
    // Queen moves in 8 directions (horizontal, vertical, diagonal)
    int directions[8][2] = {
        {0, 1}, {0, -1}, {-1, 0}, {1, 0},    // Rook-like moves
        {-1, 1}, {1, 1}, {-1, -1}, {1, -1}   // Bishop-like moves
    };
    
    for (int i = 0; i < 8; i++) {
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

