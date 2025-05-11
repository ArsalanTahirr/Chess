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
    int **directions;
    int ROWS;
    int COLS;
    if(isPiecePinnedDiagonally(board)) {
        return validMoves; // No valid moves if pinned diagonally
    }
    else if(isPiecePinnedHorizontally(board)) {
        int directionX = (board->getKingPosition(getColor()).X > position.X) ? 1 : -1;

        for (int i = 0; i < 2; ++i) {
            directions[i] = new int[2];
            directions[i][0] = directionX * (i == 0 ? 1 : -1);
            directions[i][1] = 0;
        }
        ROWS = 2;
        COLS = 2;
    }
    else if(isPiecePinnedVertically(board)) {
        int directionY = (board->getKingPosition(getColor()).Y > position.Y) ? 1 : -1;

        for (int i = 0; i < 2; ++i) {
            directions[i] = new int[2];
            directions[i][0] = 0;
            directions[i][1] = directionY * (i == 0 ? 1 : -1);
        }
        ROWS = 2;
        COLS = 2;
    }
    else {
        directions = new int*[4];
        for (int i = 0; i < 8; ++i) {
            directions[i] = new int[2];
        }
        directions[0][0] = -1; directions[0][1] = 0; // Up
        directions[1][0] = 1; directions[1][1] = 0; // Down
        directions[2][0] = 0; directions[2][1] = -1; // Left
        directions[3][0] = 0; directions[3][1] = 1; // Right  
        ROWS = 4;
        COLS = 2;
    }

    for (int i = 0; i < ROWS; i++) {
        int* dir = directions[i];
        int newRow = row;
        int newCol = col;
        
        while (true) {
            newRow += dir[0];
            newCol += dir[1];
            
            if (!isInBounds(newRow, newCol)) {
                break;
            }
            
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
    std::vector<Position> validMoves;
    int row = position.X;
    int col = position.Y;
    int **directions;
    int ROWS;
    int COLS;
        
    directions = new int*[4];
    for (int i = 0; i < 8; ++i) {
        directions[i] = new int[2];
    }
    directions[0][0] = -1; directions[0][1] = 0; // Up
    directions[1][0] = 1; directions[1][1] = 0; // Down
    directions[2][0] = 0; directions[2][1] = -1; // Left
    directions[3][0] = 0; directions[3][1] = 1; // Right  
    ROWS = 4;
    COLS = 2;

    for (int i = 0; i < ROWS; i++) {
        int* dir = directions[i];
        int newRow = row;
        int newCol = col;
        
        while (true) {
            newRow += dir[0];
            newCol += dir[1];
            
            if (!isInBounds(newRow, newCol)) {
                break;
            }
            
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
    for (const Position& move : validMoves) {
        if (move.X == to.X && move.Y == to.Y) {
            return true;
        }
    }
    return false;
}